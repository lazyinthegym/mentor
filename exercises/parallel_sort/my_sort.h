#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <fstream>
#include "timer.h"
#include "logger.h"
#include "utils.h"
#include "Thread.h"
#include <atomic>

namespace MySort {

/** Stats **/
std::atomic<int> count(0);
std::atomic<long long> total_length(0);

double get_average() {
  return total_length.load() / static_cast<double>(count.load());
}

void add_to_average(int string_length) {
  // Increment the count of strings atomically
  count++;

  // Update the total length atomically
  total_length += string_length;
}

// To test multiple ways of comparison
bool less_than_or_equal(const std::string& a, const std::string& b) {
//  return a <= b; // case #1
//  return a <= b.c_str(); // case #2
//  return strcmp(a.c_str(), b.c_str()) <= 0; // case #3
  return a.compare(b.c_str()) <= 0; // case #4, This one is the fastest
}

bool less_than(const std::string& a, const std::string& b) {
  return a.compare(b.c_str()) < 0; // case #4, This one is the fastest
}

std::vector<std::string> read_shuffled_words() {
  // Input file path
  auto filePath = std::string(RESOURCES_PATH) + "shuffledWords.txt"; // Replace with your file path

// Open the input file
  std::ifstream inputFile(filePath);

  if (!inputFile.is_open()) {
    std::cerr << "Error: Failed to open input file." << std::endl;
    exit(1);
  }

// Create a vector to store lines as strings
  std::vector<std::string> lines;

// Read lines from the file and store them in the vector
  std::string line;
  while (std::getline(inputFile, line)) {
    lines.push_back(line);
  }

// Close the input file
  inputFile.close();

  return lines;
}

void merge_sub_ranges(std::vector<std::string> &arr, std::pair<int, int> first, std::pair<int, int> second) {
  // Assert assumptions
  assert(first.second < second.first);
  assert(first.second + 1 == second.first);

  int merged_size = (second.second - first.first + 1);
  std::vector<std::string> merged(merged_size);

  int i = first.first;
  int j = second.first;
  int k = 0;

  while (i <= first.second && j <= second.second) {
    if (less_than_or_equal(arr[i], arr[j])) {
      merged[k] = std::move(arr[i]);
      i++;
    } else {
      merged[k] = std::move(arr[j]);
      j++;
    }
    k++;
  }

  // Copy any remaining elements from the first subrange
  while (i <= first.second) {
    merged[k] = std::move(arr[i]);
    i++;
    k++;
  }

  // Copy any remaining elements from the second subrange
  while (j <= second.second) {
    merged[k] = std::move(arr[j]);
    j++;
    k++;
  }

  // Copy the merged result back to the original array
  for (k = 0; k < merged_size; k++) {
    add_to_average(merged[k].size());
    arr[first.first + k] = std::move(merged[k]);
  }
}

void my_sort(std::vector<std::string> &data, int num_threads) {

  // Calculate the number of elements each thread will handle
  size_t chunk_size = data.size() / num_threads;

  std::set<std::pair<int, int>> init_sub_ranges{};

  // Create and launch threads to sort sub-ranges
  std::vector<Thread> threads;
  for (int i = 0; i < num_threads; ++i) {
    int left = i * chunk_size;
    int right = (i == num_threads - 1) ? data.size() : (i + 1) * chunk_size;

    threads.emplace_back([&data, left, right]() {
      std::sort(data.begin() + left, data.begin() + right, less_than);
    });

    init_sub_ranges.emplace(left, right - 1);
  }

  // Join all sorting threads
  for (auto &t : threads) {
    t.join();
  }

  // Merge the sorted sub-ranges

  while (init_sub_ranges.size() != 1) { // This means that all sub-ranges are merged
    // Create and launch threads to merge sub-ranges
    std::vector<Thread> merging_threads;

    std::set<std::pair<int, int>> result_sub_ranges{};

    while (init_sub_ranges.size() > 1) {
      // Merge the first 2 ranges
      auto first_range = *init_sub_ranges.begin();
      auto second_range = *std::next(init_sub_ranges.begin(), 1);
      merging_threads.emplace_back([&data, first_range, second_range] {
        merge_sub_ranges(data, first_range, second_range);
      });

      merging_threads.back().setScheduling()

      // Erase the 2 ranges and put their combined range in result_sub_ranges
      init_sub_ranges.erase(init_sub_ranges.begin());
      init_sub_ranges.erase(init_sub_ranges.begin());
      result_sub_ranges.emplace(first_range.first, second_range.second);
    }

    init_sub_ranges.insert(result_sub_ranges.begin(), result_sub_ranges.end());

    // Join all merging threads
    for (auto &t : merging_threads) {
      t.join();
    }

  }
}

} // namespace MySort