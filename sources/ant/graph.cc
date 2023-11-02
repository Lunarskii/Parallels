#include "ant/graph.h"

namespace s21 {

void Graph::LoadGraphFromFile(const std::string& filename) {
  std::ifstream file(filename);

  if (file.is_open()) {
    adjacency_matrix.ReadFull(file);
    file.close();

    if (adjacency_matrix.GetRows() != adjacency_matrix.GetCols()) {
      throw std::invalid_argument("The matrix must be square");
    }

    for (size_type i = 0; i < adjacency_matrix.GetRows(); ++i) {
      for (size_type j = 0; j < adjacency_matrix.GetCols(); ++j) {
        if (i != j && adjacency_matrix[i][j] == 0) {
          adjacency_matrix[i][j] = size_type_max;
        }
      }
    }
  } else {
    throw std::runtime_error("The file cannot be opened");
  }
}

bool Graph::IsComplete() const noexcept {
  for (size_type i = 0; i < adjacency_matrix.GetRows(); ++i) {
    for (size_type j = 0; j < adjacency_matrix.GetCols(); ++j) {
      if (adjacency_matrix[i][j] == size_type_max) {
        return false;
      }
    }
  }

  return true;
}

bool Graph::IsWeighted() const noexcept {
  for (size_type i = 0; i < adjacency_matrix.GetRows(); ++i) {
    for (size_type j = 0; j < adjacency_matrix.GetCols(); ++j) {
      if (adjacency_matrix[i][j] > 1 &&
          adjacency_matrix[i][j] < size_type_max) {
        return true;
      }
    }
  }

  return false;
}

Graph::size_type Graph::GetWeight(size_type vertex_a,
                                  size_type vertex_b) const {
  return adjacency_matrix[vertex_a][vertex_b];
}

bool Graph::IsLoaded() const noexcept {
  return adjacency_matrix.GetRows() > 0 && adjacency_matrix.GetCols() > 0;
}

Graph::AdjacencyMatrix Graph::GetAdjacencyMatrix() const noexcept {
  return adjacency_matrix;
}

Graph::size_type Graph::GetSize() const noexcept {
  return adjacency_matrix.GetRows();
}

}  // namespace s21
