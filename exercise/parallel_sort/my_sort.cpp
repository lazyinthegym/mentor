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

namespace {

// To test multiple ways of comparison
bool less_than_or_equal(const std::string& a, const std::string& b) {
//  return a <= b; // case #1
//  return a <= b.c_str(); // case #2
  return strcmp(a.c_str(), b.c_str()) <= 0; // case #3, this one is the fastest by a small margin
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
      std::sort(data.begin() + left, data.begin() + right);
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

} // namespace

/***********************************
***      Correctness Tests      ***
***********************************/
TEST(MySort, CorrectnessTest) {
  std::vector<std::string> arr = {"kiwi", "apple", "date", "cherry", "grape", "lemon", "banana", "fig", "juice"};
  my_sort(arr, 4);
  EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));

  // Verify the merged result
  std::vector<std::string> expected = {"apple", "banana", "cherry", "date", "fig", "grape", "juice", "kiwi", "lemon"};
  EXPECT_EQ(arr, expected);
}

TEST(MySort, CorrectnessTestWithLargeData) {
  std::vector<std::string> arr = read_shuffled_words();
  my_sort(arr, 9);
  EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));
}

TEST(MergeSubRangesTest, CorrectnessTest) {
  // Test Case 1: Merge Sorted Sub-Ranges
  {
    std::vector<std::string> arr = {"fig", "grape", "kiwi", "lemon", "apple", "banana", "cherry", "date"};
    std::pair<int, int> first = {0, 3};
    std::pair<int, int> second = {4, 7};

    merge_sub_ranges(arr, first, second);

    // Verify that the merged sub-ranges are sorted correctly
    EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));

    // Verify the merged result
    std::vector<std::string> expected = {"apple", "banana", "cherry", "date", "fig", "grape", "kiwi", "lemon"};
    EXPECT_EQ(arr, expected);
  }

  // Test Case 2: Merge Sorted Sub-Ranges
  {
    std::vector<std::string> arr = {"apple", "kiwi", "cherry", "date"};
    std::pair<int, int> first = {0, 1};
    std::pair<int, int> second = {2, 3};

    merge_sub_ranges(arr, first, second);

    // Verify that the merged sub-ranges are sorted correctly
    EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));

    // Verify the merged result
    std::vector<std::string> expected = {"apple", "cherry", "date", "kiwi"};
    EXPECT_EQ(arr, expected);
  }

  // Test Case 2: Merge Single Element Ranges
  {
    std::vector<std::string> arr = {"banana", "apple", "cherry", "date"};
    std::pair<int, int> first = {0, 0};
    std::pair<int, int> second = {1, 1};

    merge_sub_ranges(arr, first, second);

    // Verify the merged result
    std::vector<std::string> expected = {"apple", "banana", "cherry", "date"};
    EXPECT_EQ(arr, expected);
  }
}

/***********************************
***      Performance Tests      ***
***********************************/

TEST(MySort, CompareToSingleThread) {
  std::vector<std::string> arr_1 = read_shuffled_words();
  std::vector<std::string> arr_2 = arr_1;

  Timer timer_1;
  std::sort(arr_1.begin(), arr_1.end());
  auto std_time = timer_1.elapsed_milliseconds();

  Timer timer_2;
  my_sort(arr_2, std::thread::hardware_concurrency() - 1);
  auto algo_time = timer_2.elapsed_milliseconds();
  EXPECT_TRUE(std::is_sorted(arr_2.begin(), arr_2.end()));

  std::cout << "The Speed Up = " << std_time / algo_time << std::endl;

}

TEST(MySort, SingleRun) {
  std::vector<std::string> arr = read_shuffled_words();
  my_sort(arr, std::thread::hardware_concurrency() - 1);
  EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));
}

TEST(MySort, MultipleRuns) {
  auto num_threads = std::thread::hardware_concurrency() - 1; // Use the number of available CPU cores minus this thread
  Logger logger("my_sort_multiple_runs.txt");
  std::vector<double> times;

  std::vector<std::string> data = read_shuffled_words();

  for ( int i = 1; i <= 30; i++) {
    auto arr = data;

    Timer timer;
    my_sort(arr, num_threads);
    auto elapsed = timer.elapsed_milliseconds();

    times.emplace_back(elapsed);

    logger << "Run #" << i << ", Time = " << elapsed << " ms" << std::endl;
  }

  double rounded_average = std::round(Utils::calculateMean(times) * std::pow(10, 5)) / std::pow(10, 5);
  logger << "Average = " << rounded_average << " Standard Deviation = " << Utils::calculateStandardDeviation(times) << " ms" << std::endl;
}