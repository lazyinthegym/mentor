#include <iostream>
#include <thread>
#include "Thread.h"
#include "timer.h"
#include "logger.h"

using namespace std;

int main() {
  vector<Thread> threads;

  for (int i = 1; i <= 11; i++) {
    threads.emplace_back(
        [] {
          Timer timer;
          while (timer.elapsed_seconds() < 20) {
          }
        }
    );
    threads.back().set_max_priority();
  }

  for (auto &t : threads) {
    t.join();
  }

  return 0;
}
