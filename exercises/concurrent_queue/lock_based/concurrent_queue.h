#ifndef MENTOR_EXERCISES_CONCURRENT_QUEUE_CONCURRENT_QUEUE_H_
#define MENTOR_EXERCISES_CONCURRENT_QUEUE_CONCURRENT_QUEUE_H_

#include "queue"

class ConcurrentQueue {
 public:
  ConcurrentQueue() = default;

  void push(const int& item) {
    std::lock_guard<std::mutex> lock(mutex_);
    queue_.push(item);
  }

  std::optional<int> pop() {
    std::lock_guard<std::mutex> lock(mutex_);

    // if empty, return an empty optional
    if(queue_.empty()) return {};

    auto top = queue_.front();
    queue_.pop();
    return top;
  }

  bool isEmpty() {
    std::lock_guard<std::mutex> lock(mutex_);
    return queue_.empty();
  }

 private:
  std::queue<int> queue_;
  std::mutex mutex_;
};

#endif //MENTOR_EXERCISES_CONCURRENT_QUEUE_CONCURRENT_QUEUE_H_
