#ifndef A2_SIMPLENAVIGATOR_INCLUDE_TESTS_TEST_FILES_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_TESTS_TEST_FILES_H_

namespace Test {

struct File {
  static constexpr const char* kNonOrientedWeightedMatrix5x5 =
      "./tests/materials/non_oriented_weighted_matrix_5x5.txt";
  static constexpr const char* kNonOrientedWeightedMatrix10x10 =
      "./tests/materials/non_oriented_weighted_matrix_10x10.txt";
  static constexpr const char* kGaussMatrix3x4 =
      "./tests/materials/gauss_matrix_3x4.txt";
  static constexpr const char* kGaussMatrix11x12 =
      "./tests/materials/gauss_matrix_11x12.txt";
  static constexpr const char* kGaussMatrix1000x1001 =
      "./tests/materials/gauss_matrix_1000x1001.txt";
};

}  // namespace Test

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_TESTS_TEST_FILES_H_
