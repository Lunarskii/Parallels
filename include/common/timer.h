#ifndef A2_SIMPLENAVIGATOR_INCLUDE_COMMON_TIMER_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_COMMON_TIMER_H_

#include <chrono>
#include <functional>
#include <future>

namespace s21 {

class Timer {
 public:
  Timer(long N = 1);

  template <class Function, class... Args>
  auto MarkTime(Function&& f, Args&&... args) -> decltype(f(args...)) {
    auto func =
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...);
    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = func();
    for (std::size_t i = 0; i < static_cast<std::size_t>(N_) - 1; ++i) {
      func();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    time = std::chrono::duration_cast<std::chrono::milliseconds>(end_time -
                                                                 start_time)
               .count();

    return result;
  }

  std::chrono::milliseconds::rep GetTime() const;

 private:
  long N_;
  std::chrono::milliseconds::rep time;
};

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_COMMON_TIMER_H_
