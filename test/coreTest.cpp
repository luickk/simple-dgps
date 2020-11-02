#include <iostream>
#include "simpleDGps.h"

using namespace std;

int main() {
  int t = 3;
  double **testMatrixA = allocate2Ddouble(t, t);
  testMatrixA[0][0] = 6;
  testMatrixA[0][1] = 1;
  testMatrixA[0][2] = 1;

  testMatrixA[1][0] = 4;
  testMatrixA[1][1] = -2;
  testMatrixA[1][2] = 5;

  testMatrixA[2][0] = 2;
  testMatrixA[2][1] = 8;
  testMatrixA[2][2] = 7;

  std::cout << "transpose:" << std::endl;
  double **testTranspose = transpose2DimMatrix(testMatrixA, t, t);
  for (int i = 0; i < t; ++i)
  {
      for (int j = 0; j < t; ++j)
      {
          std::cout << testTranspose[i][j] << ' ';
      }
      std::cout << std::endl;
  }

  std::cout << "Calc Inverse:" << std::endl;
  double **testInverse = calcInverse(testMatrixA, t);
  for (int i = 0; i < t; ++i)
  {
      for (int j = 0; j < t; ++j)
      {
          std::cout << testInverse[i][j] << ' ';
      }
      std::cout << std::endl;
  }

  std::cout << "Calc Determinant:" << std::endl;
  std::cout << clacDeterminant(testMatrixA, t) << std::endl;

  // basic test
  double **testMMultiply = multiplyMatrices(testMatrixA, testMatrixA, t);
  std::cout << "Mat Mul:" << std::endl;
  for (int i = 0; i < t; ++i)
  {
      for (int j = 0; j < t; ++j)
      {
          std::cout << testMMultiply[i][j] << ' ';
      }
      std::cout << std::endl;
  }

  // trillat test
  satRanges testPseudoRanges;
  testPseudoRanges.ranges.insert({1, 23399263.4});
  testPseudoRanges.ranges.insert({2, 22473250.5});
  testPseudoRanges.ranges.insert({16, 21269160.8});

  satLocation testSatPos;
  testSatPos.locations.insert({1, ecefPos{-17104.3, -5228.5, 19811.5}});
  testSatPos.locations.insert({2, ecefPos{-3772.1, -26415.5, 117.6}});
  testSatPos.locations.insert({16, ecefPos{2600.4, -25804.1, 5853.2 }});

  ecefPos res = trillatPosFromRange(testSatPos, testPseudoRanges);

  std::cout << "Test trillat:" << std::endl;
  cout << res.x << endl;
  cout << res.y << endl;
  cout << res.z << endl;
  return 0;
}
