#include "chrono"

#ifndef CPPTRADER_SRC_TIMER_H_
#define CPPTRADER_SRC_TIMER_H_

class Timer {
 public:
  Timer() : start_time(std::chrono::high_resolution_clock::now()) {}

  void reset() {
    start_time = std::chrono::high_resolution_clock::now();
  }

  double elapsed_milliseconds() const {
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time);
    return duration.count() / 1000.0; // Convert to seconds
  }

 private:
  std::chrono::time_point<std::chrono::high_resolution_clock> start_time;
};

#endif //CPPTRADER_SRC_TIMER_H_
