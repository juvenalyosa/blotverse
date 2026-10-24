#pragma once

#include <cstddef>
#include <functional>

namespace blotverse {

// Number of worker threads to use when the caller passes 0.
unsigned default_thread_count();

// Runs body(i) for i in [0, n) on up to `threads` threads (0 = all cores).
// Results must be written to per-index slots so the output does not depend on
// scheduling. The first exception thrown by any task is rethrown.
void parallel_for(std::size_t n, const std::function<void(std::size_t)>& body,
                  unsigned threads = 0);

}  // namespace blotverse
