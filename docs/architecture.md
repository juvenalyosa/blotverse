# Architecture

BlotVerse is a six-step chain from raw image to report, with the multiverse
engine in the middle. Simulated blots enter at the same point as real images,
so the validation harness measures the tool exactly as a laboratory uses it.

```
 [1] raw image ......... io/tiff            16-bit TIFF; 8-bit warned; JPEG rejected; SHA-256
 [2] quality control ... imaging/qc         saturation, bit depth, compression, linear range
 [3] lanes & bands ..... imaging/detect     automatic detection, manual annotation override
 [4] multiverse ........ pipeline           (a) per-branch quantification cache
                         analysis/*         (b) loading -> normalisation -> scale -> test
 [5] aggregator ........ analysis/aggregate R, effect distribution, influence, joint test, verdict
 [6] report ............ report/*           HTML + CSV + reproducibility file

 sim/simulator ------> [1]       known-truth blots
 spec/multiverse.yaml -> [4]     frozen, pre-registered decisions and exclusion rules
 validation/harness <- [5]       AUC, type I error, influence hit rate (E1-E5)
```

## Source layout

| Path | Module | Responsibility |
|---|---|---|
| `include/blotverse/core` | core | `Image`, deterministic `Rng` (platform-independent distributions), `parallel_for`, numeric helpers |
| `include/blotverse/io` | io | TIFF read/write (libtiff), SHA-256 |
| `include/blotverse/imaging` | imaging | rolling-ball background (ImageJ port), lane/band detection, band quantification, QC |
| `include/blotverse/analysis` | analysis | spec parsing and enumeration, normalisation, statistical tests, multiverse engine, aggregation |
| `include/blotverse/experiment.hpp` | experiment | experiment file (YAML): blots, lanes, groups, hints, manual annotations |
| `include/blotverse/pipeline.hpp` | pipeline | steps [2]-[5] wired together |
| `include/blotverse/sim` | sim | synthetic blots with ground truth |
| `include/blotverse/report` | report | HTML report, CSV tables, reproducibility file |
| `include/blotverse/validation` | validation | level-1 harness (E1-E5) and decision point G2 |
| `apps/` | CLI | `simulate`, `detect`, `run`, `reproduce`, `validate`, `spec`, `inspect` and the validation benches |
| `tests/` | tests | doctest unit and integration tests, CLI smoke tests |

## Key design rules

* **The spec is data, not code.** Branches, the default universe, exclusion
  rules, QC thresholds, verdict thresholds and seeds live in
  `spec/multiverse.yaml`. The universe count (432 / 216) follows from it.
* **Image work happens once per branch.** For each blot and background branch
  the corrected image is computed once; each band definition is then applied to
  it. The 432 universes and every permutation of the joint test only recombine
  these cached numbers, so the joint test reruns the statistics only.
* **Determinism.** All randomness flows from seeds in the spec through
  `derive_seed`; distributions are implemented in-house because
  `std::*_distribution` differs between standard libraries. Parallel loops
  write to per-index slots, so results do not depend on the thread count
  (tested).
* **No best universe.** Nothing in the code ranks universes or reports the one
  with the smallest p-value. The default universe is fixed in the spec.
* **Exclusions are logged, not hidden.** Saturated lanes are removed from every
  universe with a QC warning; incoherent combinations (local background with
  overlapping neighbours, total protein without a total-protein image) are
  listed in the report.

## Statistical details

* **R** = fraction of valid universes whose effect has the default's sign and
  p < alpha (plan section 6). Universes that cannot be evaluated (e.g. a
  normalisation undefined for the layout) are reported, not counted.
* **Effect** is the log2 fold change in every universe: the difference of mean
  log2 ratios on the log scale, `log2((mean_c + estimate) / mean_c)` on the
  linear scale.
* **Mixed model**: random gel intercept, REML profiled over the variance ratio
  (grid + golden section, boundary at 0 checked); t inference with nlme's
  containment degrees of freedom. Matches statsmodels `MixedLM` (tests).
* **Joint test**: labels permuted *within blots* (every normalisation needs the
  per-blot design intact), the statistical part of all universes rerun, and the
  number of universes significant in the dominant direction compared with the
  null distribution. When all distinct within-blot relabellings fit in the
  permutation budget they are enumerated (exact p-value); otherwise Monte
  Carlo. When there are too few relabellings for p < alpha to be attainable
  (2 / relabellings >= alpha), the verdict is *Not evaluable*: a design limit,
  not evidence against the effect.
* **Design rules** (spec v1.1): the permutation-test branch is excluded when its
  smallest attainable p-value is >= alpha, and blot-sum / optimal-alignment
  normalisation are excluded when a blot has a single lane of some group.
* **Influence**: OLS of lfc on decision dummies; eta^2 of a decision is the
  drop-one increase in residual sum of squares divided by the total.
