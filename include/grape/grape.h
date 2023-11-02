#ifndef A2_SIMPLENAVIGATOR_INCLUDE_GRAPE_GRAPE_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_GRAPE_GRAPE_H_

#include <atomic>

#include "common/matrix.h"
#include "common/thread_pool.h"

namespace s21 {

class Grape {
 public:
  Grape() = default;
  Grape(const Matrix<double>& m1, const Matrix<double>& m2);
  ~Grape() = default;
  void LoadMatrices(const Matrix<double>& m1, const Matrix<double>& m2);
  Matrix<double> Mul();

 private:
  Matrix<double> m1_;
  Matrix<double> m2_;
};

class ClassicParallelGrape {
 public:
  ClassicParallelGrape() = default;
  ClassicParallelGrape(const Matrix<double>& m1, const Matrix<double>& m2);
  ~ClassicParallelGrape() = default;
  void LoadMatrices(const Matrix<double>& m1, const Matrix<double>& m2);
  Matrix<std::atomic<double>>& Mul(std::size_t number_of_threads);

 private:
  Matrix<std::atomic<double>> m1_;
  Matrix<std::atomic<double>> m2_;
  Matrix<std::atomic<double>> X_;
  std::vector<std::atomic<double>> row_factor_;
  std::vector<std::atomic<double>> column_factor_;
};

class PipelineParallelGrape {
 public:
  PipelineParallelGrape() = default;
  PipelineParallelGrape(const Matrix<double>& m1, const Matrix<double>& m2);
  void LoadMatrices(const Matrix<double>& m1, const Matrix<double>& m2);
  Matrix<std::atomic<double>>& Mul(std::size_t number_of_threads);

 private:
  double CalculateRowFactor(std::size_t row);
  void TryCalculateColumnFactor(std::size_t column);
  void CalculateOneMatrixElement(std::size_t row, std::size_t column,
                                 const double& row_factor);
  void TryCalculateOddMatrix(std::size_t row, std::size_t column);

 private:
  std::condition_variable cv;
  std::mutex mtx;
  std::vector<bool> col_factor_flag;
  Matrix<std::atomic<double>> m1_;
  Matrix<std::atomic<double>> m2_;
  Matrix<std::atomic<double>> X_;
  std::vector<std::atomic<double>> row_factor_;
  std::vector<std::atomic<double>> column_factor_;
};

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_GRAPE_GRAPE_H_
