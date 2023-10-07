#ifndef MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_
#define MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_

#include <iostream>
#include <tuple>
#include <string>
#include <fstream>
#include <set>

namespace Triangles {
using Triangle = std::vector<double>;


inline auto read_unique_triangles() {
  std::set<Triangle> triangles;

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
      triangles.emplace(std::vector<double>{x, y, z});
    } else {
      std::cerr << "Error reading line: " << line << std::endl;
    }
  }

  inputFile.close();

  return triangles;
}

} // namespace Triangles
#endif //MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_
