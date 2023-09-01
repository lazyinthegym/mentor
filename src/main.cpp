#include <iostream>
#include <thread>
#include <vector>
#include <cmath>

using namespace std;

int main() {

  vector<int> matrix = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50,
                    55, 60, 65, 70, 75, 80, 85, 90, 95, 100,
                    105, 110, 115, 120, 125};

  int DIM = sqrt(matrix.size());
  assert(DIM*DIM == matrix.size());

  int number_of_threads = 1;
  int result[1] = {0}; // Initialize result array for each thread
  vector<thread> threads(number_of_threads);

  for (int p = 0; p < number_of_threads; ++p) {
    auto lambda = [&, p] {
      int chunkSize = DIM/number_of_threads + 1;
      int myStart = p * chunkSize;
      int myEnd = min(myStart + chunkSize, DIM);

      for (int i = myStart; i < myEnd; ++i) {
        for (int j = 0; j < DIM; ++j) {
          if (matrix[i * DIM + j] % 2 != 0)
            ++result[p];
        }
      }

    };

    threads[p] = thread(lambda);
  }

  for (auto &thread : threads)
    thread.join();

  // Finally, do the sequential "reduction" step
  // to combine the results
  int odds = 0;
  for (int p = 0; p < number_of_threads; ++p)
    odds += result[p];

  cout << odds;
  return 0;
}

//int main () {
//  int DIM = 5;
//  int matrix[25] = {5, 10, 15, 20, 25, 30, 35, 40, 45, 50,
//                    55, 60, 65, 70, 75, 80, 85, 90, 95, 100,
//                    105, 110, 115, 120, 125};
//
//  int odds = 0;
//  for( int i = 0; i < DIM; ++i )
//    for( int j = 0; j < DIM; ++j )
//      if( matrix[i*DIM + j] % 2 != 0 )
//        ++odds;
//
//  cout << odds;
//}