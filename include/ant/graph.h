#ifndef A2_SIMPLENAVIGATOR_INCLUDE_ANT_GRAPH_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_ANT_GRAPH_H_

#include <filesystem>
#include <fstream>
#include <limits>
#include <memory>

#include "common/matrix.h"

namespace s21 {

class Graph {
 public:
  using size_type = std::size_t;
  using AdjacencyMatrix = Matrix<size_type>;
  static constexpr size_type size_type_max =
      std::numeric_limits<size_type>::max();

  void LoadGraphFromFile(const std::string& filename);

  [[nodiscard]] bool IsComplete() const noexcept;
  [[nodiscard]] bool IsLoaded() const noexcept;
  [[nodiscard]] bool IsWeighted() const noexcept;
  [[nodiscard]] AdjacencyMatrix GetAdjacencyMatrix() const noexcept;
  [[nodiscard]] size_type GetWeight(size_type vertex_a,
                                    size_type vertex_b) const;
  [[nodiscard]] size_type GetSize() const noexcept;

 private:
  AdjacencyMatrix adjacency_matrix;
};

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_ANT_GRAPH_H_
