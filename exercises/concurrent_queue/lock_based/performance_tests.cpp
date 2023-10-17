#include "gtest/gtest.h"
#include <thread>
#include <string>
#include "atomic"


#include "concurrent_queue.h"
#include "timer.h"
#include "logger.h"
#include "utils.h"

TEST(ConcurrentQueue, SingleThreadPerformanceTest) {
  ConcurrentQueue concurrent_queue;

  Timer timer;
  for (int i = 0; i < 1000000; i++) {
    concurrent_queue.push(rand());
  }

  while (!concurrent_queue.isEmpty()) {
    concurrent_queue.pop();
  }

  std::cout << "Time = " << timer.elapsed_milliseconds() << " ms" << std::endl;
}

TEST(ConcurrentQueue, MultiThreadPerformanceTest) {
  ConcurrentQueue concurrent_queue;
  Logger logger("concurrent_queue_multi_thread_test.txt", false);
  std::atomic<long> num_consumed{0};

  int num_threads = 8;
  std::vector<std::thread> threads{};
  threads.reserve(num_threads);

  // Define work for producers and consumers
  auto producer = [&concurrent_queue]() {
    for (int i = 0; i < 1000000; i++) {
      concurrent_queue.push(rand());
    }
  };

  auto consumer = [&logger,&concurrent_queue, &num_consumed]() {
    std::string thread_id =  Utils::to_string(std::this_thread::get_id());

    std::optional<int> item = concurrent_queue.pop();
    while (item) {
      // Print log in file
       num_consumed++;
       std::ostringstream ss;
       ss << "Thread " << thread_id << " Consuming item " << item.value() << std::endl;
       logger << ss.str();

       // get the next item
       item = concurrent_queue.pop();
    }

  };

  // Add some initial items first to hte queue to make sure that
  // the consumers don't start and die quickly without finding anything to consume
  producer();
  producer();


  /********** Start the test *************/
  /***************************************/
  Timer timer;
  // Start producers
  for(int th = 0; th <= num_threads/2; th++) {
    threads.emplace_back(producer);
  }

  // Start consumers
  for(int th = 0; th <= num_threads/2; th++) {
    threads.emplace_back(consumer);
  }

  // Join all threads
  for (auto& th : threads){
    th.join();
  }

  std::cout << "Time = " << timer.elapsed_milliseconds() << " ms, Consumed = " << num_consumed << std::endl;
}
