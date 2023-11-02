#ifndef A2_SIMPLENAVIGATOR_INCLUDE_COMMON_MATRIX_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_COMMON_MATRIX_H_

#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <limits>
#include <random>
#include <sstream>
#include <vector>

namespace s21 {

template <class T>
class Matrix {
 public:
  using i_type = std::size_t;
  using base = std::vector<T>;
  inline static double fp_compare_precision = 1e-6;

  Matrix() = default;
  Matrix(i_type rows, i_type cols);
  explicit Matrix(i_type rows, i_type cols, T value);
  Matrix(i_type rows, i_type cols, const base& data);
  Matrix(i_type rows, i_type cols, base&& data);

  T& operator()(i_type row, i_type col);
  const T& operator()(i_type row, i_type col) const;

  [[nodiscard]] i_type GetRows() const;
  i_type GetRows();
  [[nodiscard]] i_type GetCols() const;
  i_type GetCols();

  struct ReadRow;
  struct WriteRow;
  WriteRow operator[](i_type row);
  ReadRow operator[](i_type row) const;

  bool operator==(const Matrix& other) const;
  bool operator!=(const Matrix& other) const;

  void Print(std::ostream& os = std::cout) const;
  void PrintFull(std::ostream& os = std::cout) const;
  void ReadFull(std::istream& is = std::cin);
  void Generate(i_type rows, i_type cols);
  void LoadMatrixFromFile(std::string filename);

  void SwapRows(int i, int j);

 private:
  i_type rows_{};
  i_type cols_{};
  base data_;
};

template <class T>
void Matrix<T>::SwapRows(int i, int j) {
  if (i == j || i < 0 || j < 0 || i >= static_cast<int>(GetRows()) ||
      j >= static_cast<int>(GetRows()))
    throw std::runtime_error("Error in swap");
  for (int k = 0, ke = GetCols(); k < ke; ++k) {
    std::swap(data_[i * GetCols() + k], data_[j * GetCols() + k]);
  }
}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols) : rows_(rows), cols_(cols) {
  std::vector<T> new_vector(rows_ * cols_);
  data_ = std::move(new_vector);
}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols, T value)
    : rows_(rows), cols_(cols), data_(rows * cols, value) {}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols, const base& data)
    : rows_(rows), cols_(cols), data_(data) {}

template <class T>
Matrix<T>::Matrix(i_type rows, i_type cols, base&& data)
    : rows_(rows), cols_(cols), data_(data) {}

template <class T>
struct Matrix<T>::ReadRow {
  const Matrix& m_;
  i_type row_;

  ReadRow(const Matrix& matrix, i_type row) : m_(matrix), row_(row) {}

  const T& operator[](i_type col) const { return m_(row_, col); }
};

template <class T>
struct Matrix<T>::WriteRow {
  Matrix& m_;
  i_type row_;

  WriteRow(Matrix& matrix, i_type row) : m_(matrix), row_(row) {}

  T& operator[](i_type col) { return m_(row_, col); }
};

template <class T>
typename Matrix<T>::WriteRow Matrix<T>::operator[](i_type row) {
  return WriteRow(*this, row);
}

template <class T>
typename Matrix<T>::ReadRow Matrix<T>::operator[](i_type row) const {
  return ReadRow(*this, row);
}

template <class T>
T& Matrix<T>::operator()(i_type row, i_type col) {
  return data_[row * cols_ + col];
}

template <class T>
const T& Matrix<T>::operator()(i_type row, i_type col) const {
  return data_[row * cols_ + col];
}

template <class T>
bool Matrix<T>::operator==(const Matrix& other) const {
  if (rows_ != other.rows_ || cols_ != other.cols_) {
    return false;
  }
  if constexpr (std::is_floating_point_v<T>) {
    for (i_type i = 0; i < rows_; ++i) {
      for (i_type j = 0; j < cols_; ++j) {
        if (std::abs(data_[cols_ * i + j] - other.data_[cols_ * i + j]) >
            fp_compare_precision) {
          return false;
        }
      }
    }
  } else {
    for (i_type i = 0; i < rows_; ++i) {
      for (i_type j = 0; j < cols_; ++j) {
        if (data_[cols_ * i + j] != other.data_[cols_ * i + j]) {
          return false;
        }
      }
    }
  }
  return true;
}

template <class T>
bool Matrix<T>::operator!=(const Matrix& other) const {
  return *this != other;
}

template <class T>
typename Matrix<T>::i_type Matrix<T>::GetRows() {
  return rows_;
}

template <class T>
typename Matrix<T>::i_type Matrix<T>::GetRows() const {
  return rows_;
}

template <class T>
typename Matrix<T>::i_type Matrix<T>::GetCols() {
  return cols_;
}

template <class T>
typename Matrix<T>::i_type Matrix<T>::GetCols() const {
  return cols_;
}

template <class T>
void Matrix<T>::Print(std::ostream& os) const {
  for (i_type i = 0; i < rows_; ++i) {
    for (i_type j = 0; j < cols_; ++j) {
      os << data_[i * cols_ + j] << '\t';
    }
    os << '\n';
  }
}

template <class T>
void Matrix<T>::PrintFull(std::ostream& os) const {
  os << rows_ << '\t' << cols_ << '\n';
  Print(os);
}

template <class T>
void Matrix<T>::ReadFull(std::istream& is) {
  rows_ = {};
  cols_ = {};
  data_.clear();
  std::string line;
  T num = 0;

  is >> std::skipws;
  is >> rows_;
  is >> cols_;
  while (data_.size() < rows_ * cols_ && std::getline(is, line)) {
    std::istringstream iss(line);

    for (i_type j = 0; j < cols_ && iss >> num; ++j) {
      data_.push_back(num);
    }

    if (data_.size() % cols_ != 0 || data_.size() > rows_ * cols_ ||
        !iss.eof()) {
      throw std::invalid_argument("Incorrect data format");
    }
  }
}

template <class T>
void Matrix<T>::Generate(i_type rows, i_type cols) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_real_distribution<T> dist(-static_cast<T>(rows * cols),
                                         static_cast<T>(rows * cols));

  if (rows != rows_ || cols != cols_) {
    rows_ = rows;
    cols_ = cols;
    data_.resize(rows_ * cols_);
  }
  std::for_each(data_.begin(), data_.end(),
                [&dist, &gen](T& value) { value = dist(gen); });
}

template <class T>
void Matrix<T>::LoadMatrixFromFile(std::string filename) {
  std::ifstream file(filename);

  if (file.is_open()) {
    ReadFull(file);
  } else {
    throw std::runtime_error("The file cannot be opened");
  }
}

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_COMMON_MATRIX_H_
