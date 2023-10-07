#ifndef MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_
#define MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_

#include <iostream>
#include <tuple>
#include <string>
#include <fstream>
#include <set>


namespace std {

template <>
struct hash<std::vector<double>> {
  size_t operator()(const std::vector<double>& triangle) const {
    // Combine the hash values of the elements in the vector
    size_t hashValue = 0;
    for (const double& element : triangle) {
      // Hash each element and combine it with the existing hash value
      hashValue ^= std::hash<double>{}(element) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    }
    return hashValue;
  }
};

} // namespace std


namespace Triangles {
using Triangle = std::vector<double>;

inline auto read_unique_triangles() {
  std::unordered_map<Triangle, bool> triangles;

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
      triangles.emplace(std::vector<double>{x, y, z}, true);
    } else {
      std::cerr << "Error reading line: " << line << std::endl;
    }
  }

  inputFile.close();

  return triangles;
}

} // namespace Triangles
#endif //MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_
