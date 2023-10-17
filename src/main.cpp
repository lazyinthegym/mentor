#include <iostream>
#include <thread>
#include <sstream>
#include "timer.h"
#include "logger.h"
#include "utils.h"
#include <boost/interprocess/file_mapping.hpp>

using namespace std;

int main() {

  auto filePath = std::string(RESOURCES_PATH) + "triangles.txt";

  boost::interprocess::file_mapping m_file(filePath.c_str(), boost::interprocess::read_only);

  cout << Utils::to_string(this_thread::get_id());

  return 0;
}
