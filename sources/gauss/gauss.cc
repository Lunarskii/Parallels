#include "gauss/gauss.h"

#include <cassert>

namespace s21 {

Gauss::Gauss(const Matrix<double> &A, const std::vector<double> &B) {
  LoadData(A, B);
}

void Gauss::LoadData(const Matrix<double> &A, const std::vector<double> &B) {
  if (A.GetRows() == 0 || A.GetCols() == 0 || B.size() == 0) {
    throw std::invalid_argument("The matrix cannot have a size of 0");
  } else if (A.GetRows() != B.size()) {
    throw std::invalid_argument("The matrix is incomplete");
  }
  A_ = A;
  B_ = B;
  size_ = int(std::min(A_.GetRows(), A_.GetCols()));
}

std::vector<double> Gauss::Solve() {
  std::vector<double> X;
  rank_decrement = 0;

  for (std::size_t i = 0; i < static_cast<std::size_t>(size_); ++i) {
    if (std::abs(A_[i][i]) < 1e-7) {
      for (std::size_t j = i + 1; j < A_.GetRows(); ++j) {
        if (std::abs(A_[j][i]) > 1e-7) {
          A_.SwapRows(i, j);
          std::swap(B_[i], B_[j]);
          break;
        }
      }
      if (std::abs(A_[i][i]) < 1e-7) {
        ++rank_decrement;
        continue;
      }
    }
    auto ratio = A_[i][i];
    B_[i] /= ratio;
    for (std::size_t j = i; j < A_.GetCols(); ++j) {
      A_[i][j] /= ratio;
    }
    for (std::size_t j = i + 1; j < A_.GetRows(); ++j) {
      auto row_ratio = A_[j][i];

      B_[j] -= row_ratio * B_[i];
      for (std::size_t k = i; k < A_.GetCols(); ++k) {
        A_[j][k] -= row_ratio * A_[i][k];
      }
    }
  }
  if (CheckMatrix()) {
    X.resize(size_);
    for (int i = size_ - 1; i >= 0; --i) {
      X[i] = B_[i];

      for (int j = i - 1; j >= 0; --j) {
        B_[j] -= A_[j][i] * X[i];
      }
    }
  }

  return X;
}

bool Gauss::CheckMatrix() {
  unsigned rank = A_.GetRows();
  for (int i = A_.GetRows() - 1; i >= 0; --i) {
    if (std::abs(A_[i][A_.GetCols() - 1]) > 1e-7) {
      break;
    }
    if (std::abs(B_[i]) > 1e-7) return false;
    --rank;
  }
  rank -= rank_decrement;
  if (rank < A_.GetCols())
    throw std::runtime_error("infinitely many solutions");
  return true;
}

ParallelGauss::ParallelGauss(const Matrix<double> &A,
                             const std::vector<double> &B) {
  LoadData(A, B);
}

void ParallelGauss::LoadData(const Matrix<double> &A,
                             const std::vector<double> &B) {
  if (A.GetRows() == 0 || A.GetCols() == 0 || B.size() == 0) {
    throw std::invalid_argument("The matrix cannot have a size of 0");
  } else if (A.GetRows() != B.size()) {
    throw std::invalid_argument("The matrix is incomplete");
  }

  row_mutex_ = std::vector<std::mutex>(A.GetRows());
  row_cv_ = std::vector<std::condition_variable>(A.GetRows());
  row_cv_internal_ = std::vector<std::condition_variable>(A.GetCols());
  size_ = int(std::min(A.GetRows(), A.GetCols()));
  A_ = A;
  B_ = B;
}

std::vector<double> ParallelGauss::Solve() {
  Clear();
  pool_ = std::make_unique<ThreadPool>(std::thread::hardware_concurrency());

  SubtractingLines();

  pool_->Join();

  if (CheckMatrix()) {
    ReverseMotion();
    pool_->Join();
  }

  pool_.reset();
  return X_;
}

void ParallelGauss::SubtractingLines() {
  for (std::size_t i = 0; i < static_cast<std::size_t>(size_); ++i) {
    pool_->AddTask([this, i] {
      {
        std::unique_lock lk(row_mutex_[i]);
        row_cv_[i].wait(lk, [this, i] { return row_ready_[i]; });
        if (std::abs(A_[i][i]) < 1e-7) {
          for (std::size_t j = i + 1; j < A_.GetRows(); ++j) {
            std::unique_lock<std::mutex> lock(row_mutex_[j]);
            if (i > 0)
              row_cv_internal_[i - 1].wait(
                  lock, [this, i, j] { return std::abs(A_[j][i - 1]) < 1e-7; });
            if (std::abs(A_[j][i]) > 1e-7) {
              A_.SwapRows(i, j);
              std::swap(B_[i], B_[j]);
              break;
            }
          }
          if (std::abs(A_[i][i]) < 1e-7) {
            if (i + 1 < A_.GetRows() - 1) {
              row_ready_[i + 1] = true;
              row_cv_[i + 1].notify_one();
            }
            ++rank_decrement;
            return;
          }
        }
        auto ratio = A_[i][i];
        B_[i] /= ratio;
        for (int k = i; k < size_; ++k) {
          A_[i][k] /= ratio;
        }
      }
      for (std::size_t j = i + 1; j < A_.GetRows(); ++j) {
        std::unique_lock<std::mutex> lock(row_mutex_[j]);
        if (i > 0)
          row_cv_internal_[i - 1].wait(
              lock, [this, i, j] { return std::abs(A_[j][i - 1]) < 1e-7; });
        auto row_ratio = A_[j][i];
        B_[j] -= row_ratio * B_[i];
        for (std::size_t k = i; k < A_.GetCols(); ++k) {
          A_[j][k] -= row_ratio * A_[i][k];
        }
        if (j == i + 1) row_ready_[j] = true;
        lock.unlock();
        if (j == i + 1) row_cv_[j].notify_one();
        row_cv_internal_[i].notify_one();
      }
    });
  }
}

void ParallelGauss::ReverseMotion() {
  for (int i = size_ - 1; i >= 0; --i) {
    pool_->AddTask([this, i] {
      if (i == size_ - 1) {
        std::unique_lock<std::mutex> lock(row_mutex_[i]);
        X_[i] = B_[i];
        lock.unlock();
        row_cv_[i].notify_one();
        return;
      }
      {
        std::unique_lock lk(row_mutex_[i + 1]);
        row_cv_[i + 1].wait(lk, [this, i] { return X_[i + 1] != 0.0; });
      }
      std::unique_lock<std::mutex> lock(row_mutex_[i]);
      double sum = 0.0;
      for (int j = i + 1; j < size_; ++j) {
        sum += A_[i][j] * X_[j];
      }
      B_[i] -= sum;
      X_[i] = B_[i];
      lock.unlock();
      row_cv_[i].notify_one();
    });
  }
}

void ParallelGauss::Clear() {
  row_ready_.clear();
  row_ready_.resize(A_.GetRows());
  row_ready_.front() = true;
  rank_decrement = 0;
  X_.clear();
}

bool ParallelGauss::CheckMatrix() {
  unsigned rank = A_.GetRows();
  for (int i = A_.GetRows() - 1; i >= 0; --i) {
    if (std::abs(A_[i][A_.GetCols() - 1]) > 1e-7) {
      break;
    }
    if (std::abs(B_[i]) > 1e-7) {
      X_.clear();
      return false;
    }
    --rank;
  }
  rank -= rank_decrement;
  if (rank < A_.GetCols())
    throw std::runtime_error("infinitely many solutions");
  X_.resize(rank);
  return true;
}

}  // namespace s21
