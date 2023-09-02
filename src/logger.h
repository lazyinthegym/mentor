#ifndef CPPTRADER_SRC_LOGGER_H_
#define CPPTRADER_SRC_LOGGER_H_

#include "string"
#include <iostream>
#include <fstream>
#include <iomanip>

class Logger {
 public:
  explicit Logger(const std::string& file_name) : log_file(file_name, std::ios_base::app) {
    // Empty the log file
    log_file.close(); // Close the file
    log_file.open(std::string(LOGS_PATH) + file_name, std::ios::trunc); // Reopen with truncation
  }

  // Overload the << operator for various data types
  template <typename T>
  Logger& operator<<(const T& data) {
    log_data(data);
    return *this;
  }

  // Handle manipulators, such as std::endl
  Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
      log_data("\n"); // Log a newline
    }
    return *this;
  }

  void log_timestamp(const std::string& message) {
    auto now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    struct std::tm tm;
    localtime_r(&now_c, &tm);

    char buffer[20];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);

    log_file << "[" << buffer << "] " << message;
    std::cout << "[" << buffer << "] " << message;
  }

 private:
  std::ofstream log_file;

  template <typename T>
  void log_data(const T& data) {
    log_file << data;
    std::cout  << data;
  }
};;
#endif