#pragma once

#include <optional>
#include <span>
#include <vector>

#include "blotverse/analysis/spec.hpp"

namespace blotverse {

// Between-replicate normalisation strategies (Degasperi et al. 2014).
//
//  - FixedPoint:       divide each blot by the mean of its control lanes.
//  - BlotSum:          divide each blot by the sum of all its lanes.
//  - OptimalAlignment: per-blot scale factors k_b chosen by alternating least
//                      squares so that replicates of the same condition agree
//                      across blots: min sum (k_b y_i - m_c(i))^2 with the
//                      condition means m_c normalised to mean 1.
//
// `values` are loading-normalised signals, one per observation; `blot` and
// `group` give each observation's blot and group (group 0 is the control).
// Returns std::nullopt when the strategy is undefined for this layout (for
// instance a blot without control lanes under FixedPoint).
std::optional<std::vector<double>> normalize_replicates(Method method, std::span<const double> values,
                                                        std::span<const int> blot,
                                                        std::span<const int> group, int n_blots,
                                                        int n_groups);

}  // namespace blotverse
