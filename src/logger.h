#ifndef CPPTRADER_SRC_LOGGER_H_
#define CPPTRADER_SRC_LOGGER_H_

#include "string"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/pattern_formatter.h"

class Logger {
 public:
  explicit Logger(const std::string& file_name, bool std_cout = true) {

    auto full_file_path = std::string(LOGS_PATH) + file_name;

    // Delete the file if it exists
    std::remove(full_file_path.c_str());


    auto file_sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(full_file_path, true);
    spd_logger =  spdlog::logger("multi_sink", {file_sink});


    // Add the console sink if std_cout is true
    if (std_cout) {
      auto console_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
      spd_logger.sinks().push_back(console_sink);
    }

    // disable automatic EOL. I will add it later when I want to.
    auto formatter = std::make_unique<spdlog::pattern_formatter>("%v", spdlog::pattern_time_type::local, std::string(""));
    spd_logger.set_formatter( std::move(formatter) );
  }

  // Overload the << operator for various data types
  template <typename T>
  Logger& operator<<(const T& data) {
    spd_logger.info(data);
    return *this;
  }

  // Handle manipulators, such as std::endl
  Logger& operator<<(std::ostream& (*manip)(std::ostream&)) {
    if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl)) {
      spd_logger.info("\n"); // Log a newline
    }
    return *this;
  }

  /**
   * logs a new line
   */
  template <typename... Args>
  void log(spdlog::format_string_t<Args...> fmt, Args &&...args) {
    spd_logger.info(fmt, std::forward<Args>(args)...);
    spd_logger.info("\n");
  }


 private:
  spdlog::logger spd_logger = spdlog::logger("");
};;
#endif