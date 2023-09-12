#include <gtest/gtest.h>

#include <array>
#include <atomic>
#include <thread>
#include <vector>
#include "timer.h"
#include "logger.h"

/**
 * `alignas(128)` says that the variable is stored at the beginning of a cache line.
 * This makes sure that no two counters share the same cache line.
 * You can know the size of the cache line using `sysctl -a | grep cache`
 */
struct AlignedAtomic {
  alignas(64) std::atomic<int> counter{0};
};

void false_sharing(const int num_threads){
  // Number of total iterations to run
  const int num_iterations = 1 << 27;

  // Atomic integers to increment
  std::vector<std::atomic<int>> counters(num_threads);
  std::atomic<int> final_sum(0);

  // Number of elements to process per thread
  const int elements_per_thread = num_iterations / num_threads;

  // Lambda for our work
  auto work = [&](int thread_id) {
    for (int i = 0; i < elements_per_thread; i++) {
      counters[thread_id]++;
    }
    final_sum += counters[thread_id];
  };

  // Spawn threads
  std::vector<std::thread> threads;
  for(int i = 0; i < num_threads; i++) {
    threads.emplace_back(work, i);
  }

  for(auto& thread : threads) {
    thread.join();
  }
}

void false_sharing_solved(const int num_threads){
  // Number of total iterations to run
  const int num_iterations = 1 << 27;

  // Atomic integers to increment
  std::vector<AlignedAtomic> counters(num_threads);
  std::atomic<int> final_sum(0);

  // Number of elements to process per thread
  const int elements_per_thread = num_iterations / num_threads;

  // Lambda for our work
  auto work = [&](int thread_id) {
    for (int i = 0; i < elements_per_thread; i++) {
      counters[thread_id].counter++;
    }
    final_sum += counters[thread_id].counter;
  };

  // Spawn threads
  std::vector<std::thread> threads;
  threads.reserve(num_threads);
for(int i = 0; i < num_threads; i++) {
    threads.emplace_back(work, i);
  }

  for(auto& thread : threads) {
    thread.join();
  }
}
/**
 * Show false sharing effect by doing the same work with different number of threads and show the speedup.
 */
TEST(FalseSharing, Happening) {
  Logger logger("false_sharing.txt");

  // holds the time needed using a single thread.
  double single_thread_time = 0;

  // Test using number of threads from 1 to max_threads
  int max_threads = 10;
  for (int threads_num = 1; threads_num <= max_threads; threads_num++) {
    Timer timer;
    false_sharing(threads_num);

    auto time_elapsed = timer.elapsed_milliseconds();

    if (threads_num == 1) single_thread_time = time_elapsed;

    double speedUp = single_thread_time / time_elapsed;

    logger << "Number of Threads = " << threads_num << ", SpeedUp = " << speedUp << std::endl;
  }
}

/**
 * Show how solving false sharing can speed up the execution.
 */
TEST(FalseSharing, Solved) {
  Logger logger("false_sharing_solved.txt");

  // holds the time needed using a single thread.
  double single_thread_time = 0;

  // Test using number of threads from 1 to max_threads
  int max_threads = 10;
  for (int threads_num = 1; threads_num <= max_threads; threads_num++) {
    Timer timer;
    false_sharing_solved(threads_num);

    auto time_elapsed = timer.elapsed_milliseconds();

    if (threads_num == 1) single_thread_time = time_elapsed;

    double speedUp = single_thread_time / time_elapsed;

    logger << "Number of Threads = " << threads_num << ", SpeedUp = " << speedUp << std::endl;
  }
}
