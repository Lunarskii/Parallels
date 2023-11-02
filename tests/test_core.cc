#include "tests/test_core.h"

namespace Test {

bool ANT::IsUnique(const std::vector<size_type> vector) {
  std::unordered_set<size_type> unique;

  for (auto it = vector.begin(); it != --vector.end(); ++it) {
    if (unique.find(*it) != unique.end()) {
      return false;
    }
    unique.insert(*it);
  }

  return true;
}

size_type ANT::CalculateTheRouteDistance(std::vector<size_type> vector) {
  AdjacencyMatrix adjacency_matrix = graph.GetAdjacencyMatrix();
  auto it = vector.begin();
  size_type vertex1 = *(it++) - 1;
  size_type vertex2 = 0;
  size_type distance = 0;

  for (; it != vector.end(); vertex1 = vertex2) {
    vertex2 = *(it++) - 1;
    distance += adjacency_matrix[vertex1][vertex2];
  }

  return distance;
}

bool GAUSS::CompareVectors(const std::vector<double>& v1,
                           const std::vector<double>& v2) {
  if (v1.size() != v2.size()) {
    return false;
  }

  for (std::size_t i = 0; i < v1.size(); ++i) {
    if (std::abs(v1[i] - v2[i]) > 1e-6) {
      return false;
    }
  }

  return true;
}

bool GRAPE::CompareMatrices(const Matrix<double>& m1,
                            const Matrix<double>& m2) const {
  if (m1.GetRows() != m2.GetRows() || m1.GetCols() != m2.GetCols()) {
    return false;
  }

  for (std::size_t i = 0; i < m1.GetRows(); ++i) {
    for (std::size_t j = 0; j < m1.GetCols(); ++j) {
      if (m1[i][j] != m2[i][j]) {
        return false;
      }
    }
  }

  return true;
}

}  // namespace Test
