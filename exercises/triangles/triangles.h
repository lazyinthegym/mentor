#ifndef MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_
#define MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_

#include <iostream>
#include <tuple>
#include <string>
#include <fstream>
#include <set>

namespace Triangles {

struct Triangle {
  double x = 0;
  double y = 0;
  double z = 0;

  bool operator==(const Triangle& other) const {
    return x == other.x && y == other.y && z == other.z;
  }
};
}

namespace std {
template<>
struct hash<Triangles::Triangle> {
  size_t operator()(const Triangles::Triangle &triangle) const {
// Combine the hash values of the x, y, and z members
    size_t hashValue = 0;
    hashValue ^= std::hash<double>{}(triangle.x) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    hashValue ^= std::hash<double>{}(triangle.y) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    hashValue ^= std::hash<double>{}(triangle.z) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
    return hashValue;
  }
};
}

namespace Triangles {

/**
 * Note:
 * This is faster than using `sscanf(line, "%lf %lf %lf", &x, &y, &z)`
 * `sscanf` has to first interpret the pattern, which what we avoid by directly reading the string.
 */
inline bool scan_triangle(char* line, double& x, double& y, double& z) {
  char *endptr;
  x = strtod(line, &endptr);
  if (*endptr == ' ' || *endptr == '\t') {
    endptr++; // Skip the space or tab
    y = strtod(endptr, &endptr);
    if (*endptr == ' ' || *endptr == '\t') {
      endptr++; // Skip the space or tab
      z = strtod(endptr, &endptr);
      if (*endptr == '\r') {
        // Parsing successful
        return true;
      }
    }
  }

  return false;
}

// Using inline to be able to use in multiple source files without duplicate definitions issues.
inline auto read_unique_triangles() {
  std::unordered_map<Triangle, bool> triangles;

  auto filePath = std::string(RESOURCES_PATH) + "triangles.txt";

  // Open the file
  FILE *inputFile = fopen(filePath.c_str(), "r");

  if (!inputFile) {
    std::cerr << "Failed to open the file." << std::endl;
    exit(1); // Exit with an error code
  }

  /**
   * Note:
   * C++ STD streams would have been easier to use. However, they where very slow compared to the c-style functions.
   */
  char line[256];
  while (fgets(line, sizeof(line), inputFile)) {
    double x, y, z;
    if (scan_triangle(line, x, y, z)) {
      triangles[{x, y, z}] = true;
    } else {
      std::cerr << "Error reading line: " << line << std::endl;
    }
  }

  fclose(inputFile);

  return triangles;
}

} // namespace Triangles



#endif //MENTOR_EXERCISES_TRIANGLES_TRIANGLES_H_
