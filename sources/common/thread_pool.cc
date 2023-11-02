#include "common/thread_pool.h"

#include <iostream>

namespace s21 {

ThreadPool::ThreadPool(std::size_t num_of_threads) {
  for (std::size_t i = 0; i < num_of_threads; ++i) {
    threads.emplace_back([this] {
      while (true) {
        std::function<void()> task;

        {
          std::unique_lock<std::mutex> lock(mtx);
          cv.wait(lock, [this] { return !tasks.empty() || stop; });

          if (stop && tasks.empty()) {
            return;
          }

          task = std::move(tasks.front());
          tasks.pop_front();
        }

        task();
        std::lock_guard<std::mutex> lock(mtx);
        --task_count;
        cv_join.notify_one();
      }
    });
  }
}

ThreadPool::~ThreadPool() {
  {
    std::unique_lock<std::mutex> lock(mtx);
    stop = true;
  }

  cv.notify_all();

  for (std::thread& thread : threads) {
    thread.join();
  }
}

void ThreadPool::Join() {
  std::unique_lock<std::mutex> lock(mtx);
  cv_join.wait(lock, [this] { return task_count == 0 || stop; });
}

}  // namespace s21
