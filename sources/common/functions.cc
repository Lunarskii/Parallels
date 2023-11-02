#include "common/functions.h"

namespace s21 {

Matrix<double> ConvertAtomicMatrixToDouble(
    const Matrix<std::atomic<double>>& m) {
  Matrix<double> new_matrix(m.GetRows(), m.GetCols());

  for (std::size_t i = 0; i < m.GetRows(); ++i) {
    for (std::size_t j = 0; j < m.GetCols(); ++j) {
      new_matrix[i][j] = m[i][j].load();
    }
  }

  return new_matrix;
}

void GaussSplit(Matrix<double>& A, std::vector<double>& B) {
  Matrix<double> new_matrix(A.GetRows(), A.GetCols() - 1);
  B.clear();
  for (std::size_t i = 0; i < new_matrix.GetRows(); ++i) {
    B.push_back(A[i][A.GetCols() - 1]);
    for (std::size_t j = 0; j < new_matrix.GetCols(); ++j) {
      new_matrix[i][j] = A[i][j];
    }
  }
  A = new_matrix;
}

}  // namespace s21
