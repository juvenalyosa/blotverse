#include "blotverse/core/parallel.hpp"

#include <algorithm>
#include <atomic>
#include <exception>
#include <mutex>
#include <thread>
#include <vector>

namespace blotverse {

unsigned default_thread_count() {
  const unsigned hw = std::thread::hardware_concurrency();
  return hw == 0 ? 1 : hw;
}

void parallel_for(std::size_t n, const std::function<void(std::size_t)>& body,
                  unsigned threads) {
  if (n == 0) return;
  if (threads == 0) threads = default_thread_count();
  threads = static_cast<unsigned>(std::min<std::size_t>(threads, n));
  if (threads <= 1) {
    for (std::size_t i = 0; i < n; ++i) body(i);
    return;
  }

  std::atomic<std::size_t> next{0};
  std::atomic<bool> failed{false};
  std::exception_ptr error;
  std::mutex error_mutex;

  auto worker = [&] {
    while (!failed.load(std::memory_order_relaxed)) {
      const std::size_t i = next.fetch_add(1);
      if (i >= n) break;
      try {
        body(i);
      } catch (...) {
        std::lock_guard lock(error_mutex);
        if (!error) error = std::current_exception();
        failed = true;
      }
    }
  };

  std::vector<std::thread> pool;
  pool.reserve(threads);
  for (unsigned t = 0; t < threads; ++t) pool.emplace_back(worker);
  for (auto& t : pool) t.join();
  if (error) std::rethrow_exception(error);
}

}  // namespace blotverse
