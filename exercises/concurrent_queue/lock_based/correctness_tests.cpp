#include "gtest/gtest.h"
#include "concurrent_queue.h"

TEST(ConcurrentQueue, PushCorrectness) {
  ConcurrentQueue concurrent_queue;

  // An empty queue should return an empty optional
  EXPECT_FALSE(concurrent_queue.pop().has_value());

  concurrent_queue.push(10);
  concurrent_queue.push(20);
  concurrent_queue.push(30);
  concurrent_queue.push(40);

  EXPECT_EQ(10, concurrent_queue.pop());
  EXPECT_EQ(20, concurrent_queue.pop());
  EXPECT_EQ(30, concurrent_queue.pop());
  EXPECT_EQ(40, concurrent_queue.pop());

  EXPECT_EQ(false, concurrent_queue.pop().has_value());
}

