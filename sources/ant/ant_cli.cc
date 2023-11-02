#include "ant/ant.h"
#include "common/timer.h"

bool ProcessMenu();
unsigned GetChoice();
void RunAlgorithmNTimes(std::size_t n, s21::Ant& ant);

int main() {
  while (ProcessMenu()) {
  }

  return 0;
}

bool ProcessMenu() {
  static s21::Graph graph;
  static s21::Ant ant;
  static std::string filename;
  static long iterations;

  try {
    switch (GetChoice()) {
      case 1:
        std::cout << "\tEnter path and filename\n";
        std::cin >> filename;
        graph.LoadGraphFromFile(filename);
        graph.GetAdjacencyMatrix().PrintFull();
        ant.LoadGraph(graph);
        break;
      case 2:
        iterations = 0;
        std::cout << "\tEnter the number N" << std::endl;
        while (!(std::cin >> iterations)) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        RunAlgorithmNTimes(iterations, ant);
        break;
      case 3:
        std::cout << "\tExiting...\n";
        return false;
        break;
      default:
        std::cout << "\tTry again...\n";
    }
  } catch (std::exception& e) {
    std::cout << e.what() << "\n";
  }
  return true;
}

unsigned GetChoice() {
  unsigned chooser = 0;

  std::cout << "\t1. Loading the original graph from a file\n"
               "\t2. Run the algorithm N times\n"
               "\t3. Exit\n";

  if (!(std::cin >> chooser)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return chooser;
}

void RunAlgorithmNTimes(std::size_t n, s21::Ant& ant) {
  s21::Timer a_timer(n);
  s21::Timer pa_timer(n);
  auto a_result = a_timer.MarkTime([&]() { return ant.Solve(false); });
  auto pa_result = pa_timer.MarkTime([&]() { return ant.Solve(true); });
  std::cout << "\tAnt time\t\t" << a_timer.GetTime() << "ms" << std::endl;
  std::cout << "\tParallel Ant time\t" << pa_timer.GetTime() << "ms"
            << std::endl;
  std::cout << "\tDo you want to see the results of the algorithm? (y/n)"
            << std::endl;
  static char c;
  while (!(std::cin >> c)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }
  if (c == 'y' || c == 'Y') {
    std::cout << "\n\n\n"
              << "\tAnt:" << std::endl;
    auto tsp = a_result;
    for (auto& i : tsp.vertices) {
      std::cout << i << " ";
    }
    std::cout << "| distance = " << tsp.distance << " |\t";
    tsp = pa_result;
    std::cout << "\n\n\n"
              << "\tParallel Ant:" << std::endl;
    for (auto& i : tsp.vertices) {
      std::cout << i << " ";
    }
    std::cout << "| distance = " << tsp.distance << " |\t" << std::endl;
  }
}
