#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include "timer.h"
#include "logger.h"
#include "utils.h"
#include "my_sort.h"

namespace MySort {
/***********************************
***      Correctness Tests      ***
***********************************/
TEST(MySort, CorrectnessTest) {
  std::vector<std::string> arr = {"kiwi", "apple", "date", "cherry", "grape", "lemon", "banana", "fig", "juice"};
  my_sort(arr, 4);
  EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));

  // Verify the merged result
  std::vector<std::string> expected = {"apple", "banana", "cherry", "date", "fig", "grape", "juice", "kiwi", "lemon"};
  EXPECT_EQ(expected, arr);

  // Verify the statistics
  EXPECT_NEAR(4.7778, g_averageLength, 0.0001);
}

TEST(MySort, CorrectnessTestWithLargeData) {
  std::vector<std::string> arr = read_shuffled_words();
  my_sort(arr, 9);
  EXPECT_TRUE(std::is_sorted(arr.begin(), arr.end()));

  // Verify the statistics
  EXPECT_NEAR(9.4228, g_averageLength, 0.0001);
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
}
