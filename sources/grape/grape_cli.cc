#include <chrono>

#include "common/functions.h"
#include "common/timer.h"
#include "grape/grape.h"

bool ProcessMenu();
unsigned GetChoice();
void RunAlgorithmNTimes(std::size_t n, std::size_t number_of_threads,
                        s21::Matrix<double>& m1, s21::Matrix<double>& m2);

int main() {
  while (ProcessMenu()) {
  }

  return 0;
}

bool ProcessMenu() {
  static s21::Matrix<double> m1;
  static s21::Matrix<double> m2;
  static long iterations;
  static std::size_t number_of_threads;
  static std::size_t x1;
  static std::size_t y1;
  static std::size_t x2;
  static std::size_t y2;
  static std::string filename;

  try {
    switch (GetChoice()) {
      case 1:
        std::cout << "\tEnter the path and file name of the first matrix"
                  << std::endl;
        std::cin >> filename;
        m1.LoadMatrixFromFile(filename);
        std::cout << "\tEnter the path and file name of the second matrix"
                  << std::endl;
        std::cin >> filename;
        m2.LoadMatrixFromFile(filename);
        break;
      case 2:
        std::cout << "\tEnter the first matrix" << std::endl;
        m1.ReadFull();
        std::cout << "\tEnter the second matrix" << std::endl;
        m2.ReadFull();
        break;
      case 3:
        x1 = 0;
        y1 = 0;
        x2 = 0;
        y2 = 0;
        std::cout << "\tEnter the dimension of the first matrix" << std::endl;
        while (!(std::cin >> x1 >> y1)) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        m1.Generate(x1, y1);
        std::cout << "\tEnter the dimension of the second matrix" << std::endl;
        while (!(std::cin >> x2 >> y2)) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        m2.Generate(x2, y2);
        std::cout << "\tGenerated matrices:" << std::endl;
        std::cout << std::endl;
        m1.PrintFull();
        std::cout << std::endl;
        m2.PrintFull();
        std::cout << std::endl;
        break;
      case 4:
        number_of_threads = 0;
        std::cout << "\tEnter the number of threads:" << std::endl;
        while (!(std::cin >> number_of_threads)) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        break;
      case 5:
        iterations = 0;
        std::cout << "\tEnter the number N" << std::endl;
        while (!(std::cin >> iterations)) {
          std::cin.clear();
          std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        RunAlgorithmNTimes(iterations, number_of_threads, m1, m2);
        break;
      case 6:
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
               "\t2. Set matrices\n"
               "\t3. Generate matrices\n"
               "\t4. Set the number of threads for a classic parallel grape\n"
               "\t5. Run the algorithm N times\n"
               "\t6. Exit\n";

  if (!(std::cin >> chooser)) {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  }

  return chooser;
}

void RunAlgorithmNTimes(std::size_t n, std::size_t number_of_threads,
                        s21::Matrix<double>& m1, s21::Matrix<double>& m2) {
  s21::Grape grape(m1, m2);
  s21::ClassicParallelGrape classic_parallel_grape(m1, m2);
  s21::PipelineParallelGrape pipeline_parallel_grape(m1, m2);
  s21::Timer g_timer(n);
  s21::Timer cpg_timer(n);
  s21::Timer ppg_timer(n);
  auto g_result = g_timer.MarkTime([&]() { return grape.Mul(); });
  auto cpg_result = cpg_timer.MarkTime([&]() {
    return ConvertAtomicMatrixToDouble(
        classic_parallel_grape.Mul(number_of_threads));
  });
  auto ppg_result = ppg_timer.MarkTime([&]() {
    return ConvertAtomicMatrixToDouble(
        pipeline_parallel_grape.Mul(number_of_threads));
  });

  std::cout << "\tGrape time\t\t\t" << g_timer.GetTime() << "ms" << std::endl;
  std::cout << "\tClassic Parallel Grape time\t" << cpg_timer.GetTime() << "ms"
            << std::endl;
  std::cout << "\tPipeline Parallel Grape time\t" << ppg_timer.GetTime() << "ms"
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
              << "\tGrape:" << std::endl;
    g_result.PrintFull();
    std::cout << "\n\n\n"
              << "\tClassic Parallel Grape:" << std::endl;
    cpg_result.PrintFull();
    std::cout << "\n\n\n"
              << "\tPipeline Parallel Grape:" << std::endl;
    ppg_result.PrintFull();
  }
}
