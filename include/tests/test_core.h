#ifndef A2_SIMPLENAVIGATOR_INCLUDE_TESTS_TEST_CORE_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_TESTS_TEST_CORE_H_

#include <gtest/gtest.h>

#include <unordered_set>

#include "ant/ant.h"
#include "common/functions.h"
#include "gauss/gauss.h"
#include "grape/grape.h"
#include "test_files.h"

namespace Test {

using namespace s21;
using AdjacencyMatrix = Graph::AdjacencyMatrix;
using size_type = Ant::size_type;

class ANT : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
  bool IsUnique(std::vector<size_type> vector);
  size_type CalculateTheRouteDistance(std::vector<size_type> vector);

  Graph graph;
  Ant ant;
};

class GAUSS : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
  bool CompareVectors(const std::vector<double>& v1,
                      const std::vector<double>& v2);

  Gauss gauss;
  ParallelGauss parallel_gauss;
  Matrix<double> matrix;
  std::vector<double> vector;
};

class GRAPE : public ::testing::Test {
 protected:
  void SetUp() override {}
  void TearDown() override {}
  bool CompareMatrices(const Matrix<double>& m1,
                       const Matrix<double>& m2) const;

  Grape grape;
  ClassicParallelGrape classic_parallel_grape;
  PipelineParallelGrape pipeline_parallel_grape;
  Matrix<double> m1;
  Matrix<double> m2;
};

}  // namespace Test

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_TESTS_TEST_CORE_H_
