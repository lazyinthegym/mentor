#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>

// Function to merge two sorted subarrays of strings
void merge(std::vector<std::string>& arr, int left, int middle, int right) {
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
    }
    else {
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
void parallelMergeSort(std::vector<std::string>& arr, int left, int right, int numThreads) {
  if (left < right) {
    if (numThreads == 1) {
      // Use a single thread for small arrays
      std::sort(arr.begin() + left, arr.begin() + right + 1);
    }
    else {
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

int main() {
  std::vector<std::string> arr = {"apple", "banana", "cherry", "date", "fig", "grape"};

  int numThreads = std::thread::hardware_concurrency(); // Use the number of available CPU cores

  parallelMergeSort(arr, 0, arr.size() - 1, numThreads);

  // Print the sorted array
  for (const std::string& str : arr) {
    std::cout << str << " ";
  }
  std::cout << std::endl;

  return 0;
}
