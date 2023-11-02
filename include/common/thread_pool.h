#ifndef A2_SIMPLENAVIGATOR_INCLUDE_COMMON_THREAD_POOL_H_
#define A2_SIMPLENAVIGATOR_INCLUDE_COMMON_THREAD_POOL_H_

#include <deque>
#include <functional>
#include <future>
#include <mutex>
#include <thread>
#include <vector>

namespace s21 {

class ThreadPool {
 public:
  ThreadPool(std::size_t num_of_threads);
  ~ThreadPool();

  template <class Function, class... Args>
  auto AddTask(Function&& f, Args&&... args)
      -> std::future<decltype(f(args...))> {
    using return_type = decltype(f(args...));
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<Function>(f), std::forward<Args>(args)...));
    std::future<return_type> result = task->get_future();

    {
      std::lock_guard<std::mutex> lock(mtx);

      if (stop) {
        throw std::runtime_error("AddTask on stopped ThreadPool");
      }

      tasks.emplace_back([task]() { (*task)(); });
      ++task_count;
    }

    cv.notify_one();
    return result;
  }

  void Join();

 private:
  std::vector<std::thread> threads;
  std::deque<std::function<void()>> tasks;
  std::mutex mtx;
  std::condition_variable cv;
  std::condition_variable cv_join;
  unsigned task_count{};
  bool stop = false;
};

}  // namespace s21

#endif  // A2_SIMPLENAVIGATOR_INCLUDE_COMMON_THREAD_POOL_H_
