#include "tests/test_core.h"

namespace Test {

TEST_F(ANT, TSP) {
  graph.LoadGraphFromFile(File::kNonOrientedWeightedMatrix5x5);
  ant.LoadGraph(graph);
  Ant::TsmResult tsp = ant.Solve(false);
  EXPECT_TRUE(IsUnique(tsp.vertices));
  EXPECT_EQ(CalculateTheRouteDistance(tsp.vertices), tsp.distance);
}

TEST_F(ANT, TSP_PARALLEL) {
  graph.LoadGraphFromFile(File::kNonOrientedWeightedMatrix5x5);
  ant.LoadGraph(graph);
  Ant::TsmResult tsp = ant.Solve(true);
  EXPECT_TRUE(IsUnique(tsp.vertices));
  EXPECT_EQ(CalculateTheRouteDistance(tsp.vertices), tsp.distance);
}

TEST_F(ANT, TSP_EMPTY_GRAPH) { EXPECT_ANY_THROW(ant.LoadGraph(graph)); }

TEST_F(ANT, TSP_NON_COMPLETE_GRAPH) {
  graph.LoadGraphFromFile(File::kNonOrientedWeightedMatrix10x10);
  EXPECT_ANY_THROW(ant.LoadGraph(graph));
}

}  // namespace Test
