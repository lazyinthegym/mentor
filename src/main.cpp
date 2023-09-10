#include <iostream>
#include <thread>
#include <vector>
#include <cmath>
#include "logger.h"
#include "spdlog/pattern_formatter.h"

using namespace std;


int main() {
  // testing the logger

  Logger logger("test_log.txt");

  logger << "hello" << " world" << endl;

  logger.log("Hello {}, remember this number {}","Ahmad",3);

  return 0;
}
