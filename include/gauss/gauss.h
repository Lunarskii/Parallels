#ifndef A2_SIMPLENAVIGATOR_INCLUDE_GAUSS_GAUSS_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_GAUSS_GAUSS_H_

#include <condition_variable>
#include <iostream>
#include <mutex>
#include <shared_mutex>
#include <thread>
#include <vector>

#include "common/matrix.h"
#include "common/thread_pool.h"

namespace s21 {

class Gauss {
 public:
  Gauss() = default;
  Gauss(const Matrix<double>& A, const std::vector<double>& B);
  ~Gauss() = default;
  void LoadData(const Matrix<double>& A, const std::vector<double>& B);
  std::vector<double> Solve();

 private:
  Matrix<double> A_;
  std::vector<double> B_;
  unsigned rank_decrement{};
  int size_;

  bool CheckMatrix();
};

class ParallelGauss {
 public:
  ParallelGauss() = default;
  ParallelGauss(const Matrix<double>& A, const std::vector<double>& B);
  ~ParallelGauss() = default;
  void LoadData(const Matrix<double>& A, const std::vector<double>& B);
  std::vector<double> Solve();
  void SubtractingLines();
  void ReverseMotion();

 private:
  void Clear();
  bool CheckMatrix();

 private:
  std::vector<std::mutex> row_mutex_;
  std::vector<std::condition_variable> row_cv_;
  std::vector<std::condition_variable> row_cv_internal_;
  std::vector<bool> row_ready_;
  int size_;
  Matrix<double> A_;
  std::vector<double> B_;
  std::vector<double> X_;
  std::unique_ptr<ThreadPool> pool_;
  unsigned rank_decrement{};
};

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_GAUSS_GAUSS_H_
