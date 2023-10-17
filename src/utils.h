#ifndef MENTOR_SRC_UTILS_H_
#define MENTOR_SRC_UTILS_H_

#include <vector>
#include <cmath>

class Utils {

 public:
  static double calculateMean(const std::vector<double> &data) {
    double sum = 0.0;
    for (const double &value : data) {
      sum += value;
    }
    return sum / data.size();
  }

  static double calculateStandardDeviation(const std::vector<double> &data) {
    double mean = calculateMean(data);
    double sumSquaredDifferences = 0.0;

    for (const double &value : data) {
      double difference = value - mean;
      sumSquaredDifferences += difference * difference;
    }

    double variance = sumSquaredDifferences / data.size();
    return std::sqrt(variance);
  }

  static std::string to_string(std::thread::id id) {
    std::ostringstream ss;

    ss << std::this_thread::get_id();

    return ss.str();
  }
};

#endif //MENTOR_SRC_UTILS_H_
