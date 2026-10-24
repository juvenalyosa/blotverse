# Level-1 validation (simulation with known truth)

```bash
build/apps/blotverse validate --out validation/results/<name> \
  --experiments 2000 --gradient-experiments 200 --permutations 500 --seed <seed>
```

Each run takes about 4 minutes on a 10-core laptop and covers:

* 2000 simulated experiments: exactly half null; the rest have a true |log2 FC| of 0.3, 0.5
  or 1.0 with random sign.
* Clean, moderate or severe artefacts.
* Designs of 3x1, 2x2, 3x2 and 2x3 (gels x lanes per group per gel).
* 200 experiments with a planted background artefact, each simulated with and without it (E5).
* The full tool on every experiment: automatic detection, QC, the multiverse and the
  joint test. 0 of 2200 experiments failed, except one E5 experiment (seed 1) where lane detection asked for a manual annotation.

## Current results: spec v1.1.0 (sha256 `d332a710…d210`)

| Exp. | Question | seed 1 | seed 2 (new data) | Target | Result |
|---|---|---|---|---|---|
| E1 | p-value spread across universes, real effects, moderate artefacts (median orders of magnitude) | 1.20 (27 % ≥ 2) | 1.37 (32 % ≥ 2) | several orders (≥ 2) | **not met** |
| E2 | AUC of R, real vs null (evaluable designs) | **0.913** | **0.907** | >= 0.85 | met |
| E2' | AUC of R, all designs (reference) | 0.862 | 0.846 | – | – |
| E3 | Nulls labelled "Robust" | **0.7 %** | **0.5 %** | <= 5 % | met |
| E4 | Joint-test type I error (evaluable designs) | **0.038** | **0.034** | <= 0.05 | met |
| E4' | Joint-test type I error, all designs (reference) | 0.023 | 0.023 | – | – |
| E5 | Planted background artefact → background + band influence rises most (paired) | 67 % | 63 % | >= 80 % | **not met** |
| E5' | Background is the single most influential decision (reference) | 18 % | 17 % | – | – |
| E6 | Rolling-ball branches vs Fiji (r) | **1.00000** | – | >= 0.99 | met |

**G2 (E2 + E3 + E4): met for both seeds. G1 (E6 + speed): met.** The type I error of the single default analysis
is 0.041 and 0.056 (1000 nulls each).

Seed 2 never informed any design decision, so it serves as a held-out check.

Raw data for each run is in `validation/results/level1_spec1.1_seed{1,2}/records.csv`.

### Verdicts by design (seed 2, real effects)

| Design | Robust | Fragile | Not supported | Not evaluable |
|---|---|---|---|---|
| 3x1 | – | – | – | 100 % |
| 2x2 | – | – | – | 100 % |
| 3x2 | 58 % | 7 % | 35 % | – |
| 2x3 | 59 % | 10 % | 31 % | – |

In evaluable designs, nulls are labelled Robust in at most 1.6 % of experiments.

## Why E2 and E4 use evaluable designs

The joint test permutes group labels within each blot. When 2 / (number of distinct
within-blot relabellings) >= alpha, its p-value can never fall below alpha, whatever the
data. The observed labelling and its mirror image always count as extreme. Examples:

* 3x1 has 2^3 = 8 relabellings.
* 2x2 has 6^2 = 36 relabellings, so the smallest p is 0.056.

BlotVerse labels such experiments "Not evaluable" and never issues a robustness judgement
for them. Counting them in E4 would make the joint test look better calibrated than it is,
because their p-values are >= 0.056 by construction; the all-design values are shown only
as a reference. **Practical consequence:** a laboratory needs at least 41 within-blot
relabellings. Examples:

* 3 gels with 2 + 2 lanes each (216 relabellings).
* 2 gels with 3 + 3 lanes each (400 relabellings).

## Changes from spec v1.0 to v1.1 (made before freezing)

The v1.0 run (seed 1) is kept in `validation/results/level1_spec1.0_seed1`. It gave E2 0.880,
E3 1.0 % and E4 0.032. It also revealed the following problems:

1. **Monte Carlo noise in small designs.** In the 2x2 design, about one in three real effects
   was labelled "Robust". That was noise: 500 random permutations drawn from only 36 possible
   relabellings, while the exact p-value can never be below 0.056.
   *Fix:* the joint test now enumerates every relabelling whenever they fit the permutation
   budget, which gives an exact p-value, and adds the "Not evaluable" verdict.
2. **The permutation-test branch could not reach p < 0.05 with 3 replicates per group**
   (smallest p = 0.1), so it always counted against R.
   *Fix:* exclusion rule `permutation_underpowered`.
3. **Blot-sum and optimal-alignment normalisation were anti-conservative with one lane of a
   group per blot.** Welch type I error was 0.17 and 0.15, because dividing by the blot total
   correlates the two lanes. The default (fixed point) stayed at 0.048.
   *Fix:* exclusion rule `single_lane_per_group`.

With the same seed, these rules moved the all-design AUC from 0.880 to 0.862. The drop
comes entirely from the 3x1 design (0.834 → 0.764): the excluded branches had raised
significance in general, nulls included. AUC in the other designs did not change.

## E6: agreement with Fiji

Setup: Fiji 2.18.1 with ImageJ 1.54p, headless (macOS arm64), `validation/fiji/run_e6.sh`.
For every rolling-ball branch (rb1, rb2, rb4) and for clean and moderate blots (240 bands
each), Process > Subtract Background on the 32-bit image followed by the raw integrated
density of each band window gives **r = 1.00000 and a maximum relative difference of
0.0000 %** against BlotVerse; see `validation/results/e6_fiji/summary.md`.

The port reproduces ImageJ's arithmetic, including shrinking, ball trimming,
interpolation and pre-smoothing. Profile-peak areas are not compared: Fiji's Gel Analyzer
needs hand-drawn baselines.

## E1 in detail

E1 now uses real effects only. Gassmann et al. compared procedures on a blot with a real
difference, while null p-values are bounded to roughly [0.01, 1]. The v1.1 summaries were
recomputed from the existing records with `validate --recompute`; nothing was simulated
again.

Median orders of magnitude between the smallest and largest p-value across universes
(seed 1):

| | clean | moderate | severe |
|---|---|---|---|
| real effects | 0.99 | 1.20 | 1.92 |
| nulls | 0.30 | 0.46 | 0.87 |

The spread grows with artefacts and with effect size (2.6 orders at |lfc| = 1, moderate).
The Gassmann range (~5 orders) appears only in the tail (maximum 5.9). Whether the
simulator is too mild cannot be settled with public data; see the audit below.

## E5 in detail (paired diagnostic)

The same experiments were simulated with and without the planted artefact (exploration
seeds 5000+, not used for the validation runs). Change in mean eta^2:

* **Smooth gradients:** every background branch removes them, so there is nothing for
  the influence analysis to detect. A gradient of 0.5x the peak signal changes no
  decision's influence. At 2x the peak, background is the decision whose influence rises
  most in 62 % of experiments.
* **Mid-scale blotches** (uneven substrate, new simulator option `n_blotches`): the
  decision whose influence rises most is *band definition*, not background subtraction.
  Two of the three band definitions (profile peak with its straight baseline, Otsu mask)
  correct the local background themselves, so background artefacts are split between
  two decisions.

### Paired E5 (pre-specified, then run on seeds 1 and 2)

The criterion was fixed in code before any run. The planted artefact is a gradient of 1x
the peak signal plus 8 blotches (amplitude 0.4x, sigma 8-30 px). Each experiment is
simulated with and without it under the same seed. A hit means the influence of
background + band definition rises by more than that of any other decision.

* Hit rate: 67 % (seed 1) and 63 % (seed 2), below the 80 % target.
* The direction is right on average. Mean change in eta^2, seed 1:

  | Background | Band definition | Loading control | Others |
  |---|---|---|---|
  | +0.055 | +0.113 | -0.256 | ≈ 0 |

* Reference metric (background is the single top decision): 17-18 %.
* One of the 400 artefact experiments failed: lane detection found 1 of 2 lanes and asked
  for a manual annotation.

The influence analysis therefore moves the right way, but not reliably enough per
experiment to meet the 80 % target.

Conclusion: E5 as originally specified mixes two things. Background handling lives in both the
background and the band-definition decisions, so "background is the single most
influential decision" is the wrong success criterion. The simulator was not tuned to
make E5 pass.

## Audit of public raw images (realism check)

The plan proposed comparing the simulator with about 50 public raw images. None of the
three repositories it cites provides raw 16-bit acquisitions:

| Repository | What the files are | BlotVerse decision |
|---|---|---|
| Zenodo 14291079 | 8-bit CMYK figure compilations (~6000 x 7000 px) | rejected: CMYK print figure |
| Harvard Dataverse YC4PS4 | "16-bit" RGB files that hold upscaled 8-bit data (≤ 253 levels, all multiples of 257), pseudo-coloured, light background | rejected: colour image |
| Mendeley czm3p6t2xk | 8-bit greyscale "raw" membranes with only ~100 grey levels | accepted with warnings: 8-bit, strong quantisation |

As a result, BlotVerse now:

* detects upscaled 8-bit data inside 16-bit files;
* warns about strong quantisation;
* accepts RGB files only when R = G = B;
* names CMYK files explicitly;
* flags inverted images, which `invert: true` corrects.

The realism check, and therefore a fair E1, needs the level-2 BlotVerse-Bench
acquisitions. This also matters for level 3: deposited "raw" images are often processed.

## Open items

* **E1:** needs real 16-bit acquisitions (level 2) to decide whether the simulator is too
  mild. Public repositories do not provide them.
* **E5:** the redefined, pre-specified paired criterion reaches 63-67 %, below 80 %. On
  average the direction is right (see above), but individual experiments are too noisy.
  Next steps: more replicates per experiment, or a criterion on the average shift across
  experiments. Either must be fixed before the next run.
