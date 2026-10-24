#include <doctest/doctest.h>

#include <cmath>
#include <random>
#include <vector>

#include "blotverse/core/image.hpp"
#include "blotverse/core/numeric.hpp"
#include "blotverse/core/parallel.hpp"
#include "blotverse/core/rng.hpp"

using namespace blotverse;

TEST_CASE("Rng wraps the standard mt19937_64 engine") {
  // The 10000th output of a default-seeded mt19937_64 is fixed by the C++ standard.
  Rng rng(std::mt19937_64::default_seed);
  std::uint64_t v = 0;
  for (int i = 0; i < 10000; ++i) v = rng.next_u64();
  CHECK(v == 9981545732273789042ULL);
}

TEST_CASE("Rng distributions have the right moments") {
  Rng rng(42);
  constexpr int n = 200000;
  double sn = 0, sn2 = 0, sp = 0, sp2 = 0, sbig = 0, sl = 0, su = 0;
  for (int i = 0; i < n; ++i) {
    const double z = rng.normal();
    sn += z;
    sn2 += z * z;
    const double p = static_cast<double>(rng.poisson(4.0));
    sp += p;
    sp2 += p * p;
    sbig += static_cast<double>(rng.poisson(5000.0));
    sl += rng.lognormal_cv(0.2);
    su += rng.uniform();
  }
  CHECK(sn / n == doctest::Approx(0.0).epsilon(0.01));
  CHECK(sn2 / n == doctest::Approx(1.0).epsilon(0.01));
  CHECK(sp / n == doctest::Approx(4.0).epsilon(0.01));
  CHECK(sp2 / n - (sp / n) * (sp / n) == doctest::Approx(4.0).epsilon(0.03));
  CHECK(sbig / n == doctest::Approx(5000.0).epsilon(0.001));
  CHECK(sl / n == doctest::Approx(1.0).epsilon(0.01));
  CHECK(su / n == doctest::Approx(0.5).epsilon(0.01));
}

TEST_CASE("Rng is deterministic and derive_seed separates streams") {
  Rng a(7), b(7);
  for (int i = 0; i < 100; ++i) CHECK(a.normal() == b.normal());
  CHECK(derive_seed(1, 0) != derive_seed(1, 1));
  CHECK(derive_seed(1, 0) == derive_seed(1, 0));
}

TEST_CASE("quantile, median and variance") {
  const std::vector<double> v{1, 2, 3, 4, 10};
  CHECK(median(v) == 3.0);
  CHECK(quantile(v, 0.25) == 2.0);
  CHECK(quantile(v, 0.9) == doctest::Approx(7.6));  // NumPy linear interpolation
  CHECK(variance(v) == doctest::Approx(12.5));
  CHECK(std::isnan(mean(std::vector<double>{})));
}

TEST_CASE("binomial_capped") {
  CHECK(binomial_capped(12, 6, 100000) == 924);
  CHECK(binomial_capped(6, 3, 100000) == 20);
  CHECK(binomial_capped(60, 30, 1000) == 1000);
  CHECK(binomial_capped(3, 5, 10) == 0);
}

TEST_CASE("roc_auc handles separation and ties") {
  CHECK(roc_auc(std::vector<double>{3, 4}, std::vector<double>{1, 2}) == 1.0);
  CHECK(roc_auc(std::vector<double>{1, 2}, std::vector<double>{3, 4}) == 0.0);
  CHECK(roc_auc(std::vector<double>{1, 1}, std::vector<double>{1, 1}) == 0.5);
  CHECK(roc_auc(std::vector<double>{2, 3}, std::vector<double>{1, 2}) == doctest::Approx(0.875));
}

TEST_CASE("otsu_threshold separates two populations") {
  std::vector<float> v;
  for (int i = 0; i < 100; ++i) v.push_back(10.0f + static_cast<float>(i % 5));
  for (int i = 0; i < 50; ++i) v.push_back(200.0f + static_cast<float>(i % 7));
  const double t = otsu_threshold(v);
  CHECK(t > 14.0);
  CHECK(t <= 200.0);
}

TEST_CASE("parallel_for covers every index and propagates exceptions") {
  std::vector<int> hit(1000, 0);
  parallel_for(hit.size(), [&](std::size_t i) { hit[i] += 1; }, 8);
  for (int h : hit) CHECK(h == 1);
  CHECK_THROWS(parallel_for(10, [](std::size_t i) { if (i == 5) throw std::runtime_error("x"); }, 4));
}

TEST_CASE("Rect helpers") {
  const Rect r{2, 2, 6, 5};
  CHECK(r.width() == 4);
  CHECK(r.intersects(Rect{5, 4, 9, 9}));
  CHECK_FALSE(r.intersects(Rect{6, 0, 9, 9}));
  CHECK(r.clipped(4, 4).x1 == 4);
  ImageF img(8, 8, 1.0f);
  CHECK(sum_rect(img, r) == 12.0);
}
