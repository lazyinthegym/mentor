#ifndef MENTOR_SRC_THREAD_H_
#define MENTOR_SRC_THREAD_H_

#include <thread>
#include <pthread.h>
#include <iostream>
#include <cstring>
#include <functional>

/**
 * A POSIX compatible thread that supports setting the priority and the policy
 */
class Thread : public std::thread
{
 public:

  template <typename... Args>
  explicit Thread(Args &&...args) : std::thread(std::forward<Args>(args)...) {};

  void setScheduling(int policy, int priority) {
    sch_params.sched_priority = priority;
    if(pthread_setschedparam(this->native_handle(), policy, &sch_params)) {
      std::cerr << "Failed to set Thread scheduling : " << std::strerror(errno) << std::endl;

      // something is wrong
      exit(2);
    }
  }

 private:
  sched_param sch_params{};
};

#endif //MENTOR_SRC_THREAD_H_
