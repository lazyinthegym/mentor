#include <iostream>
#include <thread>
#include <sstream>
#include "Thread.h"
#include "timer.h"
#include "logger.h"

using namespace std;

auto read_triangles() {
  std::vector<std::tuple<double, double, double>> triangles;

  auto filePath = std::string(RESOURCES_PATH) + "triangles.txt";


  // Open the file
  std::ifstream inputFile(filePath);

  if (!inputFile.is_open()) {
    std::cerr << "Failed to open the file." << std::endl;
    exit(1); // Exit with an error code
  }

  std::string line;
  while (std::getline(inputFile, line)) {
    std::istringstream iss(line);
    double x, y, z;
    if (iss >> x >> y >> z) {
      triangles.emplace_back(x, y, z);
    } else {
      std::cerr << "Error reading line: " << line << std::endl;
    }
  }

  inputFile.close();

  return triangles;
}

int main() {

  auto triangles = read_triangles();

  return 0;
}
