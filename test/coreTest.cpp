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


  // double** multiplyMatrices(double **matrixA, double **matrixB, int matrixArows);
  // double** transpose2DimMatrix(double inputArr[][posMTrillatAColumSize]);
  // double** getCofactor(double A[][posMTrillatAColumSize], int p, int q, int n);
  // double clacDeterminant(double A[][posMTrillatAColumSize], int n);
  // double** calcInverse(double A[][posMTrillatAColumSize], int matrixArows);
  // double** calcAdjoint(double A[][posMTrillatAColumSize], int matrixArows);


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
  testPseudoRanges.ranges.insert({20, 21606540.7});
  testPseudoRanges.ranges.insert({25, 21200953.2});
  testPseudoRanges.ranges.insert({14, 23681041.7});
  testPseudoRanges.ranges.insert({6, 24601072.2});

  satLocation testSatPos;
  testSatPos.locations.insert({1, ecefPos{-17104.3, -5228.5, 19811.5}});
  testSatPos.locations.insert({2, ecefPos{-3772.1, -26415.5, 117.6}});
  testSatPos.locations.insert({16, ecefPos{2600.4, -25804.1, 5853.2 }});
  testSatPos.locations.insert({20, ecefPos{-13061.1, -14974.6, 17541.7}});
  testSatPos.locations.insert({25, ecefPos{8151.9, -13641.5, 21624.2}});
  testSatPos.locations.insert({14, ecefPos{20887.1, -16159.8, 3069.0}});
  testSatPos.locations.insert({6, ecefPos{22666.0, -2990.0, 13842.9}});

  ecefPos res = trillatPosFromRange(testSatPos, testPseudoRanges);

  std::cout << "Test trillat:" << std::endl;
  cout << res.x << endl;
  cout << res.y << endl;
  cout << res.z << endl;
  return 0;
}
