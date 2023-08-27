#ifndef CPPTRADER_SRC_LOGGER_H_
#define CPPTRADER_SRC_LOGGER_H_

#include "string"
#include <iostream>
#include <fstream>
#include <iomanip>

class Logger  {
 public:
  explicit Logger(const std::string& file_name) : log_file(file_name, std::ios_base::app) {}

  void log(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct std::tm tm;
    localtime_r(&now_c, &tm);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);

    log_file << "[" << buffer << "] " << message << std::endl;
    std::cout << "[" << buffer << "] " << message;
  }

 private:
  std::ofstream log_file;
};
#endif