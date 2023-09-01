#include <gtest/gtest.h>
#include "thread"
#include <random>

#include "../src/timer.h"
#include "../src/logger.h"

using namespace std;

vector<int> generate_matrix(int size) {
  std::vector<int> random_numbers(size);
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<int> distribution(1, 10000);

  for (int &number : random_numbers) {
    number = distribution(gen);
  }

  return random_numbers;
}

int false_sharing(int number_of_threads) {
  vector<int> matrix = generate_matrix(100);

  int DIM = sqrt(matrix.size());
  assert(DIM*DIM == matrix.size());

  int result[1] = {0}; // Initialize result array for each thread
  vector<thread> threads(number_of_threads);

  for (int p = 0; p < number_of_threads; ++p) {
    auto lambda = [&, p] {
      int chunkSize = DIM/number_of_threads + 1;
      int myStart = p * chunkSize;
      int myEnd = min(myStart + chunkSize, DIM);

      for (int i = myStart; i < myEnd; ++i) {
        for (int j = 0; j < DIM; ++j) {
          if (matrix[i * DIM + j] % 2 != 0)
            ++result[p];
        }
      }

    };

    threads[p] = thread(lambda);
  }

  for (auto &thread : threads)
    thread.join();

  // Finally, do the sequential "reduction" step
  // to combine the results
  int odds = 0;
  for (int p = 0; p < number_of_threads; ++p)
    odds += result[p];

  return odds;
}

TEST(HelloTest, BasicAssertions) {
  Logger logger("false_sharing_log.txt");

  int max_threads = 9;
  double single_thread_time = 0;

  for (int threads_num = 1; threads_num < max_threads + 1; threads_num++) {
    Timer timer;
    false_sharing(threads_num);
    std::stringstream ss;

    auto time_elapsed = timer.elapsed_seconds();

    if (threads_num == 1) {
      single_thread_time = time_elapsed;
    }

    double speedUp = single_thread_time / time_elapsed;

    ss << "Number of Threads = " << threads_num << ", speedUp = " << speedUp << std::endl;
    logger.log(ss.str());
  }

}