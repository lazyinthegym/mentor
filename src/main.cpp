#include <iostream>
#include <thread>
#include "Thread.h"
#include "timer.h"
#include "logger.h"

using namespace std;

int main() {
  std::mutex m;
  int sum = 0;
  atomic<int> sum2(0);

  Timer timer;
  for(int i = 0; i < 10000000; i++) {
    std::lock_guard<std::mutex> lock(m);
    sum++;
  }
  cout << "Mutex time = " << timer.elapsed_milliseconds() << " ms" << endl;



  Timer timer2;
  for(int i = 0; i < 10000000; i++) {
    sum2++;
  }
  cout << "Atomic time = " << timer2.elapsed_milliseconds() << " ms" << endl;

  return 0;
}
