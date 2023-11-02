#include "common/functions.h"
#include "common/timer.h"
#include "gauss/gauss.h"

bool ProcessMenu();
unsigned GetChoice();
void RunAlgorithmNTimes(std::size_t n, s21::Matrix<double>& A,
                        std::vector<double>& B);

int main() {
  while (ProcessMenu()) {
  }

  return 0;
}

bool ProcessMenu() {
  static s21::Matrix<double> A;
  static std::vector<double> B;
  static long iterations;
  static std::string filename;

  try {
    switch (GetChoice()) {
      case 1:
        std::cout << "\tEnter the path and file name" << std::endl;
        std::cin >> filename;
        A.LoadMatrixFromFile(filename);
        GaussSplit(A, B);
        break;
      case 2:
        std::cout << "\tEnter matrix" << std::endl;
        A.ReadFull();
        break;
      case 3:
        iterations = 0;
        std::cout << "\tEnter the number N" << std::endl;
        while (!(std::cin >> iterations)) {
        }
        RunAlgorithmNTimes(iterations, A, B);
        break;
      case 4:
        std::cout << "\tExiting..." << std::endl;
        return false;
        break;
      default:
        std::cout << "\tTry again..." << std::endl;
    }
  } catch (const std::exception& e) {
    std::cout << e.what() << std::endl;
  }

  return true;
}

unsigned GetChoice() {
  unsigned chooser = 0;

  std::cout << "\t1. Load a matrix from a file\n"
               "\t2. Set the matrix\n"
               "\t3. Run the algorithm N times\n"
               "\t4. Exit\n";

  if (!(std::cin >> chooser)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return chooser;
}

void RunAlgorithmNTimes(std::size_t n, s21::Matrix<double>& A,
                        std::vector<double>& B) {
  s21::Gauss gauss(A, B);
  s21::ParallelGauss parallel_gauss(A, B);
  s21::Timer g_timer(n);
  s21::Timer pg_timer(n);
  auto g_result = g_timer.MarkTime([&]() { return gauss.Solve(); });
  auto pg_result = pg_timer.MarkTime([&]() { return parallel_gauss.Solve(); });

  std::cout << "\tGauss time\t\t\t" << g_timer.GetTime() << "ms" << std::endl;
  std::cout << "\tParallel Gauss time\t" << pg_timer.GetTime() << "ms"
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
              << "Gauss:" << std::endl;
    for (auto& i : g_result) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
    std::cout << "\n\n\n"
              << "Parallel Gauss:" << std::endl;
    for (auto& i : pg_result) {
      std::cout << i << " ";
    }
    std::cout << std::endl;
  }
}
