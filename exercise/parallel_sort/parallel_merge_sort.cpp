#include <gtest/gtest.h>
#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include <fstream>
#include "timer.h"
#include "logger.h"
#include "utils.h"

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

// Function to merge two sorted sub-arrays of strings
void merge(std::vector<std::string> &arr, int left, int middle, int right) {
  int n1 = middle - left + 1;
  int n2 = right - middle;

  std::vector<std::string> leftArr(n1);
  std::vector<std::string> rightArr(n2);

  for (int i = 0; i < n1; i++)
    leftArr[i] = arr[left + i];
  for (int i = 0; i < n2; i++)
    rightArr[i] = arr[middle + 1 + i];

  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (leftArr[i] <= rightArr[j]) {
      arr[k] = leftArr[i];
      i++;
    } else {
      arr[k] = rightArr[j];
      j++;
    }
    k++;
  }

  while (i < n1) {
    arr[k] = leftArr[i];
    i++;
    k++;
  }

  while (j < n2) {
    arr[k] = rightArr[j];
    j++;
    k++;
  }
}

// Parallel Merge Sort function for strings
void parallelMergeSort(std::vector<std::string> &arr, int left, int right, int numThreads) {
  if (left < right) {
    if (numThreads == 1) {
      // There are no more available threads, sort this chunk in a single thread
      std::sort(arr.begin() + left, arr.begin() + right + 1);
      return;
    }

    int middle = left + (right - left) / 2;

    // Create two threads for sorting the left and right halves
    std::thread leftThread(parallelMergeSort, std::ref(arr), left, middle, numThreads / 2);
    std::thread rightThread(parallelMergeSort, std::ref(arr), middle + 1, right, numThreads / 2);

    // Wait for both threads to finish
    leftThread.join();
    rightThread.join();

    // Merge the sorted halves
    merge(arr, left, middle, right);

  }
}
}

TEST(ParallelMergeSort, CalculateSpeedUp) {

  std::vector<std::string> data = read_shuffled_words();

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
  parallelMergeSort(data_2, 0, data_2.size() - 1, num_threads - 1);
  auto multi_thread_time = multi_timer.elapsed_milliseconds();

  std::cout << "Using a Parallel Merge Sort with "<< num_threads << " threads, the speed up = "  << single_thread_time/multi_thread_time << std::endl;

  //Compare with the std::sort result to exercise its correctness
  EXPECT_EQ(data_1, data_2);
}

TEST(ParallelMergeSort, SingleRun) {
  std::vector<std::string> data = read_shuffled_words();

  auto num_threads = std::thread::hardware_concurrency(); // Use the number of available CPU cores
  parallelMergeSort(data, 0, data.size() - 1, num_threads);
}

TEST(ParallelMergeSort, MultipleRuns) {
  auto num_threads = std::thread::hardware_concurrency() - 1; // Use the number of available CPU cores minus this thread
  Logger logger("multiple_merge_sort_runs.txt");
  std::vector<double> times;

  std::vector<std::string> data = read_shuffled_words();

  for ( int i = 1; i <= 20; i++) {
    auto arr = data;

    Timer timer;
    parallelMergeSort(arr, 0, arr.size() - 1, num_threads);
    auto elapsed = timer.elapsed_milliseconds();

    times.emplace_back(elapsed);

    logger << "Run #" << i << ", Time = " << elapsed << " ms" << std::endl;
  }

  double rounded_average = std::round(Utils::calculateMean(times) * std::pow(10, 5)) / std::pow(10, 5);
  logger << "Average = " << rounded_average << " Standard Deviation = " << Utils::calculateStandardDeviation(times) << " ms" << std::endl;
}