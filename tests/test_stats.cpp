#include <doctest/doctest.h>

#include <vector>

#include "blotverse/analysis/stats.hpp"

using namespace blotverse;

// Reference values computed with SciPy 1.17 and statsmodels (see scripts/stats_reference.py).

TEST_CASE("incomplete beta and Student t match SciPy") {
  CHECK(regularized_incomplete_beta(2.5, 1.5, 0.3) == doctest::Approx(0.0889437231706656).epsilon(1e-10));
  CHECK(regularized_incomplete_beta(0.5, 7, 0.9) == doctest::Approx(0.999999978070216).epsilon(1e-10));
  CHECK(student_t_two_sided_p(2.1, 7.3) == doctest::Approx(0.0722467134248534).epsilon(1e-10));
  CHECK(student_t_two_sided_p(0.5, 3) == doctest::Approx(0.651447964848151).epsilon(1e-10));
  CHECK(student_t_two_sided_p(10, 30) == doctest::Approx(4.57525140822962e-11).epsilon(1e-8));
  CHECK(student_t_two_sided_p(0, 5) == doctest::Approx(1.0));
}

TEST_CASE("Welch test matches scipy.stats.ttest_ind(equal_var=False)") {
  const std::vector<double> a{1.1, 0.9, 1.3, 1.05, 0.95};
  const std::vector<double> b{1.6, 1.9, 1.4, 2.2, 1.75, 1.8};
  const TestResult r = welch_test(a, b);
  REQUIRE(r.ok);
  CHECK(r.statistic == doctest::Approx(5.46109232770924).epsilon(1e-10));
  CHECK(r.p == doctest::Approx(0.000566011418928946).epsilon(1e-9));
  CHECK_FALSE(welch_test(std::vector<double>{1, 1}, std::vector<double>{2, 2}).ok);
  CHECK_FALSE(welch_test(std::vector<double>{1}, std::vector<double>{2, 3}).ok);
}

TEST_CASE("mixed model matches statsmodels MixedLM (REML)") {
  const std::vector<double> y{0.1, 0.3, 1.2, 0.9, 0.6, 0.5, 1.4, 1.7, -0.2, 0.0, 0.8, 0.7};
  const std::vector<int> t{0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1};
  const std::vector<int> g{0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2};
  const TestResult r = mixed_model_test(y, t, g);
  REQUIRE(r.ok);
  CHECK(r.estimate == doctest::Approx(0.9).epsilon(1e-8));
  CHECK(r.se == doctest::Approx(0.0803638262262).epsilon(1e-6));
  CHECK(r.df == 8.0);  // N - gels - 1 (nlme containment)
}

TEST_CASE("mixed model handles unbalanced gels and a single gel") {
  const std::vector<double> y{1.0, 1.2, 2.1, 0.8, 1.9, 2.3, 2.2};
  const std::vector<int> t{0, 0, 1, 0, 1, 1, 1};
  const std::vector<int> g{0, 0, 0, 1, 1, 1, 1};
  const TestResult r = mixed_model_test(y, t, g);
  REQUIRE(r.ok);
  CHECK(r.estimate > 0.8);
  const std::vector<int> one{0, 0, 0, 0, 0, 0, 0};
  const TestResult single = mixed_model_test(y, t, one);
  REQUIRE(single.ok);
  CHECK(single.df == 5.0);
}

TEST_CASE("exact permutation test") {
  // 3 vs 3 completely separated: 2 of the 20 relabellings are as extreme (two-sided).
  const std::vector<double> a{1, 2, 3};
  const std::vector<double> b{10, 11, 12};
  const TestResult r = permutation_test(a, b, 10000, 1);
  REQUIRE(r.ok);
  CHECK(r.p == doctest::Approx(0.1));
  CHECK(r.estimate == doctest::Approx(9.0));
}

TEST_CASE("Monte Carlo permutation test is deterministic per seed") {
  std::vector<double> a, b;
  for (int i = 0; i < 15; ++i) {
    a.push_back(i * 0.1);
    b.push_back(0.5 + i * 0.1);
  }
  const TestResult r1 = permutation_test(a, b, 2000, 9);
  const TestResult r2 = permutation_test(a, b, 2000, 9);
  CHECK(r1.p == r2.p);
  CHECK(r1.p > 0.0);
  CHECK(r1.p < 0.2);
}

TEST_CASE("smallest attainable permutation p-value") {
  CHECK(permutation_min_p(3, 3, 10000) == doctest::Approx(0.1));        // 2 / 20
  CHECK(permutation_min_p(4, 4, 10000) == doctest::Approx(2.0 / 70.0));
  CHECK(permutation_min_p(3, 4, 10000) == doctest::Approx(1.0 / 35.0));  // no mirror image
  CHECK(permutation_min_p(15, 15, 1000) == doctest::Approx(1.0 / 1001.0));
  // Agrees with what the test actually returns for perfectly separated groups.
  const std::vector<double> a{1, 2, 3}, b{7, 8, 9};
  CHECK(permutation_test(a, b, 10000, 1).p == doctest::Approx(permutation_min_p(3, 3, 10000)));
}
