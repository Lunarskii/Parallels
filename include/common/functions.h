#ifndef A2_SIMPLENAVIGATOR_INCLUDE_COMMON_FUNCTIONS_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_COMMON_FUNCTIONS_H_

#include <atomic>

#include "matrix.h"

namespace s21 {

Matrix<double> ConvertAtomicMatrixToDouble(
    const Matrix<std::atomic<double>>& m);
void GaussSplit(Matrix<double>& A, std::vector<double>& B);

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_COMMON_FUNCTIONS_H_
