#include <iostream>
#include <sstream>
#include "timer.h"
#include "thread"
#include "logger.h"

int main() {
  Logger logger("tmpLog.txt");

  // start the timer
  Timer timer;

  std::this_thread::sleep_for(std::chrono::seconds(2));

  std::stringstream ss;
  ss << "Hello Karl, " << timer.elapsed_seconds() << " seconds has passed\n";
  logger.log(ss.str());

  return 0;
}
