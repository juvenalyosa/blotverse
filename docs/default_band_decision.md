# Spec v1.3.0: default band definition and near-saturation rule

Decided on 2026-10-25. The rule was to pick, for each question, the option that gives better
results on the evidence available, with real data taking priority over simulation. The spec
before this change was v1.2.0 (sha256 `50bbba7d…`); the spec after it is v1.3.0.

## 1. Default band definition: `profile` → `rect` (adopted)

Only the default universe changes, from `rb2/profile/hk/fixed/log/welch` to
`rb2/rect/hk/fixed/log/welch`. All branches stay in the multiverse.

**What the default affects:**

* R, which counts significant universes with the default's sign;
* the default estimate shown in reports and the GUI;
* the linear-range QC of blots that declare loads;
* the G3 check of the exposure bench.

**Evidence:**

| Benchmark | Measure | `rb2/profile` | `rb2/rect` | Better |
|---|---|---|---|---|
| Dilution series, Pillai-Kastoori 2020 (4 bands) | mean \|error\| of known log2 FC, all lane pairs | 1.40–2.39 | 1.11–1.92 | rect, in all 4 bands |
| AURORA A reanalysis (15 lanes) | median relative difference from the authors' ImageQuant values | 72 % | 26 % | rect |
| Exposure bench, development set | q95 \|Δ\| of clean pairs; median \|Δ\| of saturated pairs | 0.544; 1.92 | 0.236; 0.54 | rect |
| Exposure bench, v1.2 confirmatory set | q95 \|Δ\| of clean pairs; median \|Δ\| of saturated pairs | 0.125; 0.37 | 0.067; 0.18 | rect |
| Level-1 simulation, seed 5, 6000 + 600, paired | E2 AUC, E3, E4 (decision metrics) | 0.923 | 0.923 | equal |
| Level-1 simulation, seed 5, paired | mean \|default lfc − true lfc\|: clean / moderate / severe | 0.117 / 0.122 / 0.155 | 0.116 / 0.123 / 0.179 | equal on clean and moderate; profile on severe |

**Reading:**

* On every real-data benchmark, the rectangle is closer to the truth (or to the authors) and
  is damaged less by saturation.
* A clipped peak loses its height, while a rectangle keeps the area that is not clipped.
* In simulation the two defaults give the same verdict quality.
* The profile peak resists the strongest simulated artefacts better (blotches, uneven
  background). That cost is real but confined to severe simulated artefacts.
* Real data come first, so the default becomes `rect`.

**Validation under v1.3.0:** see `validation/results/level1_spec1.3_seed5/` and
[`validation.md`](validation.md).

**Exposure bench under v1.3.0 (sensitivity):** P1 = 0 misses in both sets. The v1.1 pair
rule drops from 4 to 1 miss (development) and from 1 to 0 (confirmatory). The exposure
results in `validation/results/exposure/` were produced under v1.2.0.

**Status of earlier confirmatory runs:**

* The seed-3 level-1 run (spec v1.1.0) stays the record of the pre-specified G2 test.
* Seed 5 under v1.3.0 checks that G2 still holds with the new default. It is not a new
  pre-registration.

## 2. Near-saturation rule (≥ 75 % of the detector maximum): not adopted

The question was whether lanes near saturation (a pixel ≥ 75 % of full scale, but none at
the maximum) distort ratios, so that flagging or excluding them would improve results.
Every series of the exposure bench was pooled (default branch, lanes above the SNR floor):

| | Lanes / pairs | \|Δ\| > 0.2 log2 |
|---|---|---|
| Near-saturated lanes, lane-level scale | 2 | 0 |
| Other unsaturated lanes | 107 | 4 |
| Clean pairs with a near-saturated lane | 3 | 0 |
| Clean pairs without | 305 | 19 |

**Result: no measurable benefit.**

* Near-saturated lanes were never distorted. The distortions that occur are in faint lanes,
  not bright ones.
* As an exclusion, the rule would discard valid data and gain nothing.
* As a warning, it would raise alarms the data do not justify.

The rule stays in the exposure bench as a candidate (`near_saturation_fraction`). If future
series show near-saturation compression the spec rule misses, it can be revisited.
