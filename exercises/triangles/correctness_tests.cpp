#include <gtest/gtest.h>
#include "triangles.h"

static double tolerance = 1e-7;
namespace Triangles {

bool isEqual(const Triangle& a, const Triangle& b) {
  if ( abs(a[0] - b[0]) < tolerance &&
      abs(a[1] - b[1]) < tolerance &&
      abs(a[2] - b[2]) < tolerance) {
    return true;
  }
  return false;
}

TEST(Triangles, LargeDataCorrectnessTest) {
  auto triangles = read_unique_triangles();
  EXPECT_EQ(triangles.count({1, 2, 3}), 0);

  // Duplicated triangle exists only once
  int count = std::count_if(triangles.begin(), triangles.end(),
                            [](const Triangle& element) { return isEqual({142.29985046, 21.15739441, -157.72848511}, element) ;});
  EXPECT_EQ(count, 1);

  EXPECT_EQ(38800, triangles.size());
}

}
