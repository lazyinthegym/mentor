#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <fstream>
#include "timer.h"

namespace {
std::vector<std::string> read_shuffled_words(){
  // Input file path
  auto filePath = std::string(RESOURCES_PATH) + "shuffledWords.txt"; // Replace with your file path

// Open the input file
  std::ifstream inputFile(filePath);

  if (!inputFile.is_open()) {
    std::cerr << "Error: Failed to open input file." << std::endl;
    exit(1) ;
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

// Function to merge two sorted sub-ranges into a single sorted sub-range
void mergeSortedRanges(std::vector<std::string>& arr, int left, int middle, int right) {
  std::vector<std::string> temp(right - left + 1);
  int i = left, j = middle + 1, k = 0;

  while (i <= middle && j <= right) {
    if (arr[i] <= arr[j]) {
      temp[k++] = arr[i++];
    } else {
      temp[k++] = arr[j++];
    }
  }

  while (i <= middle) {
    temp[k++] = arr[i++];
  }

  while (j <= right) {
    temp[k++] = arr[j++];
  }

  // Copy the merged values back into the original array
  for (i = 0; i < k; ++i) {
    arr[left + i] = temp[i];
  }
}

void parallel_sort(std::vector<std::string>& data, int num_threads) {

  // Calculate the number of elements each thread will handle
  size_t chunk_size = data.size() / num_threads;

//  assert(chunk_size * num_threads == data.size());

  // Create and launch threads to sort sub-ranges
  std::vector<std::thread> threads;
  for (int i = 0; i < num_threads; ++i) {
    int left = i * chunk_size;
    int right = (i == num_threads - 1) ? data.size() : (i + 1) * chunk_size;

    threads.emplace_back([&data, left, right](){
      std::sort(data.begin() + left, data.begin() + right);
    });
  }

  // Wait for all threads to finish
  for (std::thread& t : threads) {
    t.join();
  }

  // Merge the sorted sub-ranges (your existing code)
  for (int i = 1; i <= num_threads; ++i) {
    int middle = i * chunk_size - 1;
    int right = (i + 1) * chunk_size - 1;
    if (right >= data.size()) {
      right = data.size() - 1;
    }
    mergeSortedRanges(data, 0, middle, right);
  }
}

} // namespace

TEST(ParallelSort, CalculateSpeedUp) {
  std::vector<std::string> data = read_shuffled_words();

//  data.resize(100);

  // Create two different copies for each case
  std::vector<std::string> data_1(data);
  std::vector<std::string> data_2(data);

  // Single Thread Sorting
  Timer single_timer;
  std::sort(data_1.begin(), data_1.end());
  auto single_thread_time = single_timer.elapsed_milliseconds();

  // Parallel Merge Sorting
  int num_threads = std::thread::hardware_concurrency(); // Use the number of available CPU cores
  Timer multi_timer;
  parallel_sort(data_2, num_threads - 1);
  auto multi_thread_time = multi_timer.elapsed_milliseconds();

  std::cout << "Using a Parallel Merge Sort with "<< num_threads << " threads, the speed up = "  << single_thread_time/multi_thread_time << std::endl;

  //Compare with the std::sort result to test its correctness

  EXPECT_EQ(data_1, data_2);
}
