#include "common/timer.h"

namespace s21 {

Timer::Timer(long N) : N_(N) {
  if (N_ <= 0) {
    throw std::invalid_argument(
        "The number of iterations must be greater than or equal to 1");
  }
}

std::chrono::milliseconds::rep Timer::GetTime() const { return time; }

}  // namespace s21