#ifndef A2_SIMPLENAVIGATOR_INCLUDE_ANT_ANT_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_ANT_ANT_H_

#include <shared_mutex>
#include <vector>

#include "common/matrix.h"
#include "common/thread_pool.h"
#include "graph.h"

namespace s21 {

class Ant {
 public:
  using size_type = Graph::size_type;
  using tour_t = std::vector<size_type>;
  static constexpr size_type size_type_max = Graph::size_type_max;

  struct TsmResult {
    tour_t vertices;
    double distance{};
  };

  Ant() = default;
  void LoadGraph(Graph& graph);
  TsmResult Solve(bool parallel = false);

 private:
  tour_t ConstructTour();
  void UpdatePheromone();
  size_type CalculateTotalDistance(const tour_t& tour);
  void ProcessPheromoneUpdate();

  void AddPheromoneValue(size_type i, size_type j, double value);

 private:
  constexpr static double alpha = 1.0;  // Вес феромона
  constexpr static double beta = 2.0;   // Вес эвристики
  constexpr static double evaporation_rate =
      0.5;  // Скорость испарения феромона

 private:
  bool parallel_{};
  std::unique_ptr<ThreadPool> pool_;
  Graph graph_;
  size_type number_of_ants{};
  size_type max_iterations{};
  Matrix<double> pheromone_mx_;
  mutable std::vector<std::vector<std::mutex>> mu_;
};

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_ANT_ANT_H_
