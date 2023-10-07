#include <gtest/gtest.h>
#include "triangles.h"
#include "timer.h"

namespace Triangles {

TEST(Triangles, PerformanceTest) {

  Timer timer;
  auto triangles = read_unique_triangles();
  std::cout << "Reading Triangles: Time = " << timer.elapsed_milliseconds() << " ms" << std::endl;
}

}
