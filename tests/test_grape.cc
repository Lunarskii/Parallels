#include "tests/test_core.h"

namespace Test {

TEST_F(GRAPE, GENERATED_SQUARE_MATRICES) {
  for (unsigned i = 0; i < 10; ++i) {
    m1.Generate(10, 10);
    m2.Generate(10, 10);
    grape.LoadMatrices(m1, m2);
    classic_parallel_grape.LoadMatrices(m1, m2);
    pipeline_parallel_grape.LoadMatrices(m1, m2);
    EXPECT_TRUE(CompareMatrices(
        grape.Mul(),
        ConvertAtomicMatrixToDouble(classic_parallel_grape.Mul(6))));
    EXPECT_TRUE(CompareMatrices(
        grape.Mul(),
        ConvertAtomicMatrixToDouble(pipeline_parallel_grape.Mul(6))));
  }

  for (unsigned i = 0; i < 10; ++i) {
    m1.Generate(100, 100);
    m2.Generate(100, 100);
    grape.LoadMatrices(m1, m2);
    classic_parallel_grape.LoadMatrices(m1, m2);
    pipeline_parallel_grape.LoadMatrices(m1, m2);
    EXPECT_TRUE(CompareMatrices(
        grape.Mul(),
        ConvertAtomicMatrixToDouble(classic_parallel_grape.Mul(6))));
    EXPECT_TRUE(CompareMatrices(
        grape.Mul(),
        ConvertAtomicMatrixToDouble(pipeline_parallel_grape.Mul(6))));
  }

  //    for (unsigned i = 0; i < 10; ++i)
  //    {
  //        m1.Generate(1000, 1000);
  //        m2.Generate(1000, 1000);
  //        grape.LoadMatrices(m1, m2);
  //        classic_parallel_grape.LoadMatrices(m1, m2);
  //        pipeline_parallel_grape.LoadMatrices(m1, m2);
  //        EXPECT_TRUE(CompareMatrices(grape.Mul(),
  //        ConvertAtomicMatrixToDouble(classic_parallel_grape.Mul(6))));
  //        EXPECT_TRUE(CompareMatrices(grape.Mul(),
  //        ConvertAtomicMatrixToDouble(pipeline_parallel_grape.Mul(6))));
  //    }
}

TEST_F(GRAPE, RECTANGULAR_MATRICES) {
  m1.Generate(2, 3);
  m2.Generate(3, 2);
  grape.LoadMatrices(m1, m2);
  classic_parallel_grape.LoadMatrices(m1, m2);
  pipeline_parallel_grape.LoadMatrices(m1, m2);
  EXPECT_TRUE(CompareMatrices(
      grape.Mul(), ConvertAtomicMatrixToDouble(classic_parallel_grape.Mul(6))));
  EXPECT_TRUE(CompareMatrices(
      grape.Mul(),
      ConvertAtomicMatrixToDouble(pipeline_parallel_grape.Mul(6))));

  m1.Generate(3, 2);
  m2.Generate(2, 3);
  grape.LoadMatrices(m1, m2);
  classic_parallel_grape.LoadMatrices(m1, m2);
  pipeline_parallel_grape.LoadMatrices(m1, m2);
  EXPECT_TRUE(CompareMatrices(
      grape.Mul(), ConvertAtomicMatrixToDouble(classic_parallel_grape.Mul(6))));
  EXPECT_TRUE(CompareMatrices(
      grape.Mul(),
      ConvertAtomicMatrixToDouble(pipeline_parallel_grape.Mul(6))));

  m1.Generate(3, 5);
  m2.Generate(5, 10);
  grape.LoadMatrices(m1, m2);
  classic_parallel_grape.LoadMatrices(m1, m2);
  pipeline_parallel_grape.LoadMatrices(m1, m2);
  EXPECT_TRUE(CompareMatrices(
      grape.Mul(), ConvertAtomicMatrixToDouble(classic_parallel_grape.Mul(6))));
  EXPECT_TRUE(CompareMatrices(
      grape.Mul(),
      ConvertAtomicMatrixToDouble(pipeline_parallel_grape.Mul(6))));
}

TEST_F(GRAPE, EVEN_MANUAL_MATRICES) {
  m1 = Matrix<double>(
      10, 10, {7, 5, 0, 2, 8, 6, 9, 8, 2, 8, 8, 7, 2, 0, 5, 1, 8, 6, 5, 8,
               4, 4, 3, 5, 9, 4, 3, 4, 7, 2, 9, 9, 2, 3, 1, 7, 5, 6, 5, 0,
               4, 8, 7, 4, 2, 9, 7, 2, 3, 5, 1, 2, 4, 6, 6, 2, 8, 9, 2, 6,
               3, 0, 9, 8, 0, 3, 7, 8, 9, 7, 9, 5, 8, 8, 4, 1, 4, 3, 8, 0,
               7, 5, 5, 1, 9, 2, 4, 1, 7, 6, 5, 7, 3, 9, 8, 8, 3, 1, 7, 9});
  m2 = Matrix<double>(
      10, 10, {2, 1, 8, 2, 6, 9, 5, 0, 9, 7, 5, 6, 5, 3, 4, 2, 6, 0, 6, 1,
               3, 0, 3, 2, 6, 1, 6, 4, 7, 3, 1, 2, 6, 0, 6, 4, 7, 1, 2, 9,
               7, 3, 0, 7, 2, 6, 0, 8, 2, 9, 6, 4, 8, 2, 9, 5, 3, 5, 5, 5,
               0, 3, 0, 3, 6, 9, 2, 1, 2, 0, 0, 7, 0, 4, 6, 2, 1, 9, 6, 9,
               8, 9, 0, 1, 1, 5, 9, 2, 1, 6, 0, 5, 5, 5, 8, 9, 7, 7, 6, 3});
  Matrix<double> result(
      10, 10, {149, 230, 181, 198, 312, 338, 197, 237, 259, 282, 138, 220, 153,
               171, 260, 304, 220, 181, 248, 227, 185, 191, 133, 139, 207, 229,
               196, 176, 178, 271, 161, 202, 197, 114, 256, 236, 214, 123, 243,
               233, 165, 189, 214, 135, 304, 259, 243, 159, 249, 207, 100, 186,
               112, 154, 256, 241, 174, 215, 191, 250, 131, 224, 158, 127, 302,
               270, 286, 205, 234, 282, 173, 176, 177, 111, 237, 242, 265, 124,
               230, 280, 186, 186, 148, 159, 219, 273, 222, 172, 215, 238, 223,
               245, 247, 174, 321, 331, 305, 214, 255, 322});

  grape.LoadMatrices(m1, m2);
  classic_parallel_grape.LoadMatrices(m1, m2);
  pipeline_parallel_grape.LoadMatrices(m1, m2);
  EXPECT_TRUE(CompareMatrices(grape.Mul(), result));
  EXPECT_TRUE(CompareMatrices(
      ConvertAtomicMatrixToDouble(classic_parallel_grape.Mul(6)), result));
  EXPECT_TRUE(CompareMatrices(
      ConvertAtomicMatrixToDouble(pipeline_parallel_grape.Mul(6)), result));
}

TEST_F(GRAPE, UNEVEN_MANUAL_MATRICES) {
  m1 = Matrix<double>(5, 5, {7, 5, 0, 2, 8, 8, 7, 2, 0, 5, 4, 4, 3,
                             5, 9, 9, 9, 2, 3, 1, 4, 8, 7, 4, 2});
  m2 = Matrix<double>(5, 5, {2, 1, 8, 2, 6, 5, 6, 5, 3, 4, 3, 0, 3,
                             2, 6, 1, 2, 6, 0, 6, 7, 3, 0, 7, 2});
  Matrix<double> result(
      5, 5, {97, 65,  93, 85, 90,  92, 65,  105, 76, 98,  105, 65, 91,
             89, 106, 79, 72, 141, 56, 122, 87,  66, 117, 60,  126});

  grape.LoadMatrices(m1, m2);
  classic_parallel_grape.LoadMatrices(m1, m2);
  pipeline_parallel_grape.LoadMatrices(m1, m2);
  EXPECT_TRUE(CompareMatrices(grape.Mul(), result));
  EXPECT_TRUE(CompareMatrices(
      ConvertAtomicMatrixToDouble(classic_parallel_grape.Mul(6)), result));
  EXPECT_TRUE(CompareMatrices(
      ConvertAtomicMatrixToDouble(pipeline_parallel_grape.Mul(6)), result));
}

TEST_F(GRAPE, INCORRECT_MATRIX_SIZE) {
  EXPECT_ANY_THROW(grape.LoadMatrices(m1, m2));
  EXPECT_ANY_THROW(classic_parallel_grape.LoadMatrices(m1, m2));
  EXPECT_ANY_THROW(pipeline_parallel_grape.LoadMatrices(m1, m2));

  m1.Generate(3, 3);
  m2.Generate(5, 5);
  EXPECT_ANY_THROW(grape.LoadMatrices(m1, m2));
  EXPECT_ANY_THROW(classic_parallel_grape.LoadMatrices(m1, m2));
  EXPECT_ANY_THROW(pipeline_parallel_grape.LoadMatrices(m1, m2));

  m1.Generate(2, 3);
  m2.Generate(2, 3);
  EXPECT_ANY_THROW(grape.LoadMatrices(m1, m2));
  EXPECT_ANY_THROW(classic_parallel_grape.LoadMatrices(m1, m2));
  EXPECT_ANY_THROW(pipeline_parallel_grape.LoadMatrices(m1, m2));
}

}  // namespace Test
