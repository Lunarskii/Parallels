#include "tests/test_core.h"

namespace Test {

TEST_F(GAUSS, MATRIX_3X4) {
  matrix.LoadMatrixFromFile(File::kGaussMatrix3x4);
  GaussSplit(matrix, vector);
  gauss.LoadData(matrix, vector);
  parallel_gauss.LoadData(matrix, vector);
  EXPECT_TRUE(CompareVectors(gauss.Solve(), parallel_gauss.Solve()));
}

TEST_F(GAUSS, MATRIX_11X12) {
  matrix.LoadMatrixFromFile(File::kGaussMatrix11x12);
  GaussSplit(matrix, vector);
  gauss.LoadData(matrix, vector);
  parallel_gauss.LoadData(matrix, vector);
  EXPECT_TRUE(CompareVectors(gauss.Solve(), parallel_gauss.Solve()));
}

TEST_F(GAUSS, MATRIX_1000X1001) {
  matrix.LoadMatrixFromFile(File::kGaussMatrix1000x1001);
  GaussSplit(matrix, vector);
  gauss.LoadData(matrix, vector);
  parallel_gauss.LoadData(matrix, vector);
  EXPECT_TRUE(CompareVectors(gauss.Solve(), parallel_gauss.Solve()));
}

TEST_F(GAUSS, ONE_SOLUTION) {
  matrix = Matrix<double>(3, 3, {39, 125, 344, 39, 125, 344, 44, 480, 212});
  GaussSplit(matrix, vector);
  gauss.LoadData(matrix, vector);
  parallel_gauss.LoadData(matrix, vector);
  EXPECT_TRUE(CompareVectors(gauss.Solve(), {10.485627, -0.519515}));
  EXPECT_TRUE(CompareVectors(parallel_gauss.Solve(), {10.485627, -0.519515}));
}

TEST_F(GAUSS, INFINITE_NUMBER_OF_SOLUTIONS) {
  matrix = Matrix<double>(2, 3, {3, 3, 3, 5, 5, 5});
  GaussSplit(matrix, vector);
  gauss.LoadData(matrix, vector);
  parallel_gauss.LoadData(matrix, vector);
  EXPECT_ANY_THROW(parallel_gauss.Solve());
  EXPECT_ANY_THROW(gauss.Solve());
}

TEST_F(GAUSS, INFINITE_NUMBER_OF_SOLUTIONS2) {
  matrix = Matrix<double>(2, 4, {1, 2, 3, 4, 1, 2, 4, 5});
  GaussSplit(matrix, vector);
  gauss.LoadData(matrix, vector);
  parallel_gauss.LoadData(matrix, vector);
  EXPECT_ANY_THROW(parallel_gauss.Solve());
  EXPECT_ANY_THROW(gauss.Solve());
}

TEST_F(GAUSS, NO_SOLUTIONS) {
  matrix = Matrix<double>(2, 3, {3, 3, 2, 5, 5, 4});
  GaussSplit(matrix, vector);
  gauss.LoadData(matrix, vector);
  parallel_gauss.LoadData(matrix, vector);
  EXPECT_TRUE(parallel_gauss.Solve().empty());
  EXPECT_TRUE(gauss.Solve().empty());
}

TEST_F(GAUSS, INCOMPLETE_MATRIX) {
  matrix.LoadMatrixFromFile(File::kGaussMatrix3x4);
  GaussSplit(matrix, vector);
  vector.pop_back();
  EXPECT_ANY_THROW(gauss.LoadData(matrix, vector));
  EXPECT_ANY_THROW(parallel_gauss.LoadData(matrix, vector));
}

TEST_F(GAUSS, EMPTY_MATRIX) {
  EXPECT_ANY_THROW(gauss.LoadData(matrix, vector));
  EXPECT_ANY_THROW(parallel_gauss.LoadData(matrix, vector));
}

}  // namespace Test
