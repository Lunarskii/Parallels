#include "grape/grape.h"

namespace s21 {

Grape::Grape(const Matrix<double>& m1, const Matrix<double>& m2) {
  LoadMatrices(m1, m2);
}

void Grape::LoadMatrices(const Matrix<double>& m1, const Matrix<double>& m2) {
  if (m1.GetRows() == 0 || m1.GetCols() == 0 || m2.GetRows() == 0 ||
      m2.GetCols() == 0) {
    throw std::invalid_argument("The matrix cannot have a size of 0");
  } else if (m1.GetCols() != m2.GetRows()) {
    throw std::invalid_argument(
        "The number of columns in the first matrix should be equal to the "
        "number of rows in the second");
  }

  m1_ = m1;
  m2_ = m2;
}

Matrix<double> Grape::Mul() {
  std::vector<double> row_factor(m1_.GetRows(), 0);
  std::vector<double> column_factor(m2_.GetCols(), 0);
  Matrix<double> result(m1_.GetRows(), m2_.GetCols());

  for (std::size_t i = 0; i < m1_.GetRows(); ++i) {
    for (std::size_t j = 0; j < m1_.GetCols() / 2; ++j) {
      row_factor[i] += m1_[i][2 * j] * m1_[i][2 * j + 1];
    }
  }

  for (std::size_t i = 0; i < m2_.GetCols(); ++i) {
    for (std::size_t j = 0; j < m2_.GetRows() / 2; ++j) {
      column_factor[i] += m2_[2 * j][i] * m2_[2 * j + 1][i];
    }
  }

  for (std::size_t i = 0; i < result.GetRows(); ++i) {
    for (std::size_t j = 0; j < result.GetCols(); ++j) {
      result[i][j] = -row_factor[i] - column_factor[j];
      for (std::size_t k = 0; k < m1_.GetCols() / 2; ++k) {
        result[i][j] += (m1_[i][2 * k] + m2_[2 * k + 1][j]) *
                        (m1_[i][2 * k + 1] + m2_[2 * k][j]);
      }
    }
  }

  if (m1_.GetCols() % 2 != 0) {
    for (std::size_t i = 0; i < result.GetRows(); ++i) {
      for (std::size_t j = 0; j < result.GetCols(); ++j) {
        result[i][j] += m1_[i][m1_.GetCols() - 1] * m2_[m1_.GetCols() - 1][j];
      }
    }
  }

  return result;
}

ClassicParallelGrape::ClassicParallelGrape(const Matrix<double>& m1,
                                           const Matrix<double>& m2) {
  LoadMatrices(m1, m2);
}

void ClassicParallelGrape::LoadMatrices(const Matrix<double>& m1,
                                        const Matrix<double>& m2) {
  if (m1.GetRows() == 0 || m1.GetCols() == 0 || m2.GetRows() == 0 ||
      m2.GetCols() == 0) {
    throw std::invalid_argument("The matrix cannot have a size of 0");
  } else if (m1.GetCols() != m2.GetRows()) {
    throw std::invalid_argument(
        "The number of columns in the first matrix should be equal to the "
        "number of rows in the second");
  }

  m1_ = Matrix<std::atomic<double>>(m1.GetRows(), m1.GetCols());
  m2_ = Matrix<std::atomic<double>>(m2.GetRows(), m2.GetCols());
  X_ = Matrix<std::atomic<double>>(m1_.GetRows(), m2_.GetCols());
  row_factor_ = std::vector<std::atomic<double>>(m1.GetRows());
  column_factor_ = std::vector<std::atomic<double>>(m2.GetCols());

  for (std::size_t i = 0; i < m1.GetRows(); ++i) {
    for (std::size_t j = 0; j < m1.GetCols(); ++j) {
      m1_[i][j].store(m1[i][j]);
    }
  }
  for (std::size_t i = 0; i < m2.GetRows(); ++i) {
    for (std::size_t j = 0; j < m2.GetCols(); ++j) {
      m2_[i][j].store(m2[i][j]);
    }
  }
}

Matrix<std::atomic<double>>& ClassicParallelGrape::Mul(
    std::size_t number_of_threads) {
  ThreadPool pool(number_of_threads);

  for (std::size_t i = 0; i < m1_.GetRows(); ++i) {
    pool.AddTask([this, i]() {
      for (std::size_t j = 0; j < m1_.GetCols() / 2; ++j) {
        row_factor_[i] = row_factor_[i] + m1_[i][2 * j] * m1_[i][2 * j + 1];
      }
    });
  }

  for (std::size_t i = 0; i < m2_.GetCols(); ++i) {
    pool.AddTask([this, i]() {
      for (std::size_t j = 0; j < m2_.GetRows() / 2; ++j) {
        column_factor_[i] =
            column_factor_[i] + m2_[2 * j][i] * m2_[2 * j + 1][i];
      }
    });
  }

  pool.Join();

  for (std::size_t i = 0; i < X_.GetRows(); ++i) {
    pool.AddTask([this, i]() {
      for (std::size_t j = 0; j < X_.GetCols(); ++j) {
        X_[i][j] = -row_factor_[i] - column_factor_[j];
        for (std::size_t k = 0; k < m1_.GetCols() / 2; ++k) {
          X_[i][j] = X_[i][j] + (m1_[i][2 * k] + m2_[2 * k + 1][j]) *
                                    (m1_[i][2 * k + 1] + m2_[2 * k][j]);
        }
      }
    });
  }

  pool.Join();

  if (m1_.GetCols() % 2 != 0) {
    for (std::size_t i = 0; i < X_.GetRows(); ++i) {
      for (std::size_t j = 0; j < X_.GetCols(); ++j) {
        X_[i][j] =
            X_[i][j] + m1_[i][m1_.GetCols() - 1] * m2_[m1_.GetCols() - 1][j];
      }
    }
  }

  return X_;
}

PipelineParallelGrape::PipelineParallelGrape(const Matrix<double>& m1,
                                             const Matrix<double>& m2) {
  LoadMatrices(m1, m2);
}

void PipelineParallelGrape::LoadMatrices(const Matrix<double>& m1,
                                         const Matrix<double>& m2) {
  if (m1.GetRows() == 0 || m1.GetCols() == 0 || m2.GetRows() == 0 ||
      m2.GetCols() == 0) {
    throw std::invalid_argument("The matrix cannot have a size of 0");
  } else if (m1.GetCols() != m2.GetRows()) {
    throw std::invalid_argument(
        "The number of columns in the first matrix should be equal to the "
        "number of rows in the second");
  }

  m1_ = Matrix<std::atomic<double>>(m1.GetRows(), m1.GetCols());
  m2_ = Matrix<std::atomic<double>>(m2.GetRows(), m2.GetCols());
  X_ = Matrix<std::atomic<double>>(m1_.GetRows(), m2_.GetCols());
  row_factor_ = std::vector<std::atomic<double>>(m1.GetRows());
  column_factor_ = std::vector<std::atomic<double>>(m2.GetCols());
  col_factor_flag = std::vector<bool>(column_factor_.size());
  for (std::size_t i = 0; i < m1.GetRows(); ++i) {
    for (std::size_t j = 0; j < m1.GetCols(); ++j) {
      m1_[i][j].store(m1[i][j]);
    }
  }
  for (std::size_t i = 0; i < m2.GetRows(); ++i) {
    for (std::size_t j = 0; j < m2.GetCols(); ++j) {
      m2_[i][j].store(m2[i][j]);
    }
  }
}

double PipelineParallelGrape::CalculateRowFactor(std::size_t row) {
  double row_factor = 0;

  for (std::size_t k = 0; k < m1_.GetCols() / 2; ++k) {
    row_factor += m1_[row][2 * k] * m1_[row][2 * k + 1];
  }

  return row_factor;
}

void PipelineParallelGrape::TryCalculateColumnFactor(std::size_t column) {
  std::lock_guard<std::mutex> lock(mtx);
  if (column_factor_[column] == 0) {
    for (std::size_t k = 0; k < m2_.GetRows() / 2; ++k) {
      column_factor_[column] =
          column_factor_[column] + m2_[2 * k][column] * m2_[2 * k + 1][column];
    }
  }
}

void PipelineParallelGrape::CalculateOneMatrixElement(
    std::size_t row, std::size_t column, const double& row_factor) {
  X_[row][column] = -row_factor - column_factor_[column];
  for (std::size_t k = 0; k < m1_.GetCols() / 2; ++k) {
    X_[row][column] =
        X_[row][column] + (m1_[row][2 * k] + m2_[2 * k + 1][column]) *
                              (m1_[row][2 * k + 1] + m2_[2 * k][column]);
  }
}

void PipelineParallelGrape::TryCalculateOddMatrix(std::size_t row,
                                                  std::size_t column) {
  if (m1_.GetCols() % 2 != 0) {
    X_[row][column] = X_[row][column] + m1_[row][m1_.GetCols() - 1] *
                                            m2_[m1_.GetCols() - 1][column];
  }
}

Matrix<std::atomic<double>>& PipelineParallelGrape::Mul(
    std::size_t number_of_threads) {
  ThreadPool pool(number_of_threads);

  for (std::size_t i = 0; i < X_.GetRows(); ++i) {
    pool.AddTask([this, i]() {
      double row_factor = CalculateRowFactor(i);
      for (std::size_t j = 0; j < X_.GetCols(); ++j) {
        TryCalculateColumnFactor(j);
        CalculateOneMatrixElement(i, j, row_factor);
        TryCalculateOddMatrix(i, j);
      }
    });
  }

  //    pool.AddTask([this, &pool]
  //    {
  //        for (int j = 0; j < m1_.GetCols() / 2; ++j)
  //        {
  //            row_factor_[0] = row_factor_[0] + m1_[0][2 * j] * m1_[0][2 * j +
  //            1];
  //        }
  //        for (int j = 0; j < X_.GetCols(); ++j)
  //        {
  //            std::unique_lock<std::mutex> lk(mtx);
  //            cv.wait(lk, [this, j] { return col_factor_flag[j]; });
  //            pool.AddTask([this, j]()
  //            {
  //                X_[0][j] = -row_factor_[0] - column_factor_[j];
  //                for (int k = 0; k < m1_.GetCols() / 2; ++k)
  //                {
  //                    X_[0][j] = X_[0][j] + (m1_[0][2 * k] + m2_[2 * k +
  //                    1][j]) * (m1_[0][2 * k + 1] + m2_[2 * k][j]);
  //                }
  //            });
  //        }
  //    });
  //
  //    for (int i = 0; i < m2_.GetCols(); ++i)
  //    {
  //        pool.AddTask([this, i]()
  //        {
  //            for (int j = 0; j < m2_.GetRows() / 2; ++j)
  //            {
  //                column_factor_[i] = column_factor_[i] + m2_[2 * j][i] *
  //                m2_[2 * j + 1][i];
  //            }
  //            std::unique_lock<std::mutex> lk(mtx);
  //            column_factor_[i] = true;
  //            lk.unlock();
  //            cv.notify_one();
  //        });
  //    }
  //
  //    for (int i = 1; i < m1_.GetRows(); ++i)
  //    {
  //        pool.AddTask([this, &pool, i]()
  //        {
  //            for (int j = 0; j < m1_.GetCols() / 2; ++j)
  //            {
  //                row_factor_[i] = row_factor_[i] + m1_[i][2 * j] * m1_[i][2 *
  //                j + 1];
  //            }
  //            pool.AddTask([this, i]()
  //            {
  //                for (int j = 0; j < X_.GetCols(); ++j)
  //                {
  //                    X_[i][j] = -row_factor_[i] - column_factor_[j];
  //                    for (int k = 0; k < m1_.GetCols() / 2; ++k)
  //                    {
  //                        X_[i][j] = X_[i][j] + (m1_[i][2 * k] + m2_[2 * k +
  //                        1][j]) * (m1_[i][2 * k + 1] + m2_[2 * k][j]);
  //                    }
  //                }
  //            });
  //        });
  //    }
  //
  //    pool.Join();

  return X_;
}

}  // namespace s21
