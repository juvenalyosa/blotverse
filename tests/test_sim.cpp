#include <doctest/doctest.h>

#include <algorithm>
#include <cmath>
#include <filesystem>

#include "blotverse/experiment.hpp"
#include "blotverse/sim/simulator.hpp"

using namespace blotverse;
namespace fs = std::filesystem;

TEST_CASE("simulator layout and ground truth") {
  SimParams p = sim_preset(ArtifactLevel::Moderate);
  p.n_gels = 3;
  p.reps_per_gel = 2;
  p.true_lfc = {0.0, 1.0};
  const auto sim = simulate(p);
  REQUIRE(sim.blots.size() == 3);
  for (const auto& b : sim.blots) {
    CHECK(b.lanes.size() == 4);
    CHECK(b.image.width() == 2 * p.margin + 4 * p.lane_pitch);
    CHECK(b.image.height() == p.height);
    REQUIRE(b.total_protein);
    CHECK(b.lanes[0].group == "ctrl");
    CHECK(b.lanes[1].group == "treat");
  }
  // True amounts reflect the fold change on average.
  double ctrl = 0, treat = 0;
  for (const auto& b : sim.blots)
    for (const auto& l : b.lanes) (l.group == "ctrl" ? ctrl : treat) += std::log2(l.target_amount);
  CHECK((treat - ctrl) / 6.0 == doctest::Approx(1.0).epsilon(0.35));
}

TEST_CASE("simulator is deterministic per seed") {
  SimParams p = sim_preset(ArtifactLevel::Severe);
  p.seed = 99;
  const auto a = simulate(p);
  const auto b = simulate(p);
  CHECK(a.blots[0].image.data() == b.blots[0].image.data());
  p.seed = 100;
  CHECK(simulate(p).blots[0].image.data() != a.blots[0].image.data());
}

TEST_CASE("bands rise above background where the truth says they are") {
  SimParams p = sim_preset(ArtifactLevel::Clean);
  p.n_gels = 1;
  const auto sim = simulate(p);
  const auto& b = sim.blots[0];
  const auto& lane = b.lanes[0];
  const int x = static_cast<int>(std::lround(lane.x));
  const int y = static_cast<int>(std::lround(lane.target_y));
  CHECK(b.image(x, y) > p.offset + 0.5 * p.peak_per_unit * lane.target_amount * lane.load * b.exposure);
  CHECK(b.image(x, y - 30) < p.offset + 200);
}

TEST_CASE("detector ceiling: high exposure saturates at 65535") {
  SimParams p = sim_preset(ArtifactLevel::Clean);
  p.n_gels = 1;
  p.peak_per_unit = 200000;
  const auto sim = simulate(p);
  const auto& px = sim.blots[0].image.data();
  CHECK(*std::max_element(px.begin(), px.end()) == 65535);
}

TEST_CASE("written simulation reloads as an experiment") {
  SimParams p = sim_preset(ArtifactLevel::Clean);
  p.n_gels = 2;
  const auto sim = simulate(p);
  const fs::path dir = fs::temp_directory_path() / "blotverse_tests" / "sim_write";
  fs::remove_all(dir);
  write_simulation(dir, sim);
  CHECK(fs::exists(dir / "truth.yaml"));
  const Experiment ex = load_experiment(dir / "experiment.yaml");
  REQUIRE(ex.blots.size() == 2);
  CHECK(ex.control_group == "ctrl");
  CHECK(ex.groups() == std::vector<std::string>{"ctrl", "treat"});
  CHECK(ex.has_total_protein());
  CHECK(ex.blots[0].image.pixels.data() == sim.blots[0].image.data());
  CHECK(ex.blots[0].image.meta.sha256 == to_experiment(sim).blots[0].image.meta.sha256);
}
