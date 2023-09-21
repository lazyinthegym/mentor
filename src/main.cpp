#include <iostream>
#include <thread>
#include "Thread.h"

int main() {

  Thread th([]{std::this_thread::sleep_for(std::chrono::milliseconds(5000));});

  th.setScheduling(SCHED_FIFO, 1);

  th.join();

  return 0;
}
