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
  joint test.

## Current results: protocol v2 (confirmatory)

Criteria, intervals and decision rules were fixed in
[validation_protocol.md](validation_protocol.md) and committed before the confirmatory run
(commit `9c0eff8`). Setup:

* spec v1.1.0 (sha256 `d332a710…d210`);
* simulator presets v2, calibrated to real images ([real_data.md](real_data.md));
* comb-fit lane detection.

**Confirmatory run (seed 3):** 6000 main experiments and 600 paired E5 experiments; 0
failures. Intervals are 95 %; E5 uses Bonferroni-adjusted 98.75 %.

| Criterion | Estimate [CI] | n | Rule | Result |
|---|---|---|---|---|
| E2 AUC of R, evaluable designs | **0.925** [0.915, 0.934] | 3028 | lower bound ≥ 0.85 | **met** |
| E3 evaluable nulls labelled Robust | **1.8 %** [1.2, 2.6] | 1523 | upper bound ≤ 5 % | **met** |
| E4 joint-test type I error, evaluable nulls | **0.033** [0.026, 0.044] | 1523 | estimate ≤ 0.05, not detectably above | **met** |
| E5 mean change in background + band influence (H) | **+0.179** [0.148, 0.210] | 600 | lower bound > 0 | **met** |
| E5 margin H − change in loading | 0.508 [0.448, 0.572] | 600 | lower bound > 0 | met |
| E5 margin H − change in normalisation | 0.189 [0.158, 0.220] | 600 | lower bound > 0 | met |
| E5 margin H − change in scale | 0.233 [0.199, 0.270] | 600 | lower bound > 0 | met |
| E5 margin H − change in test | 0.179 [0.148, 0.210] | 600 | lower bound > 0 | met (trivially: see note) |
| E6 rolling-ball branches vs Fiji | r = 1.00000 (1440 bands) | | r ≥ 0.99 | **met** |

**G1, G2 and E5 are met in the confirmatory run.** The exploratory seeds 1 and 2 agree:

* E2: 0.918 [0.900, 0.935] and 0.905 [0.887, 0.924];
* E4: 0.042 [0.027, 0.064] in both;
* E5: met in both.

Seed 2 had one E5 failure: lane detection asked for a manual annotation, in a 2-lane gel
with 8 blotches.

Notes:

* **E5, "test" comparison.** The choice of statistical test barely changes the estimated
  effect, so its influence change is ≈ 0. That comparison is satisfied by construction and
  adds no evidence; the loading, normalisation and scale margins carry the result.
* **Per-experiment E5.** The protocol-v1 rule (hit rate) is 72 % [69, 76], still below
  80 %. The influence analysis points to background handling reliably on average, not in
  every single experiment.
* **E3 denominator.** Protocol v2 uses evaluable nulls only, which is stricter than the
  all-null denominator of v1.
* **Single default analysis.** Its type I error is 0.036 [0.030, 0.043].

### E1: spread of evidence across universes (descriptive, confirmatory run)

| Level | Truth | Median orders of magnitude, p_max/p_min [CI] | Universes disagree on significance [CI] |
|---|---|---|---|
| clean | real | 0.93 [0.88, 1.01] | 23.5 % [21.0, 26.2] |
| clean | null | 0.28 [0.27, 0.30] | 9.1 % [7.5, 11.0] |
| moderate | real | 1.10 [1.02, 1.20] | 23.8 % [21.2, 26.5] |
| moderate | null | 0.34 [0.33, 0.36] | 10.0 % [8.3, 12.1] |
| severe | real | 1.54 [1.45, 1.59] | **58.6 %** [55.6, 61.6] |
| severe | null | 0.65 [0.62, 0.69] | 18.1 % [15.8, 20.7] |

This is the conclusion-level analogue of Gassmann et al. (2009). Among real effects,
defensible procedures disagree about statistical significance in about one experiment in
four with typical artefacts, and in more than half with severe ones.

Raw data: `validation/results/level1_protocol2_seed{1,2}_exploratory/` and
`level1_protocol2_seed3_confirmatory/`.

### Earlier runs

The earlier runs are kept for transparency in `validation/results/*_sim1_*`. They were judged by point estimates (protocol v1).

* **Spec v1.1 with simulator presets v1** (harsher than any real image, with the old
  lane detection), seeds 1 / 2:
  * E2 0.913 / 0.907;
  * E3 0.7 / 0.5 %;
  * E4 0.038 / 0.034;
  * E5 (paired) 67 / 63 %;
  * E1 1.20 / 1.37.
* **Spec v1.1 with presets v2, protocol v1** (seeds 1 / 2; in git history at commit
  `744e498`):
  * E2 0.918 / 0.905;
  * E3 0.6 / 0.7 % of all nulls;
  * E4 0.042 / 0.042;
  * E5 hit rate 74 / 65 %;
  * E1 0.96 / 1.10.

  Seeds 1 and 2 were rerun under protocol v2 to add the significance-disagreement
  column; the simulated experiments are identical.
* **Spec v1.0**: see the next sections.

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

The v1.0 run (seed 1) is kept in `validation/results/level1_spec1.0_sim1_seed1`. It gave E2 0.880,
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

## E1 in detail (simulator presets v1)

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
The Gassmann range (~5 orders) appears only in the tail (maximum 5.9).

The realism check against real acquisitions ([real_data.md](real_data.md)) shows that these
v1 presets were *harsher* than real images, not milder. The small spread therefore reflects a
branch set that is more homogeneous than the procedures Gassmann compared, not a mild
simulator.

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

### Paired E5 (pre-specified, then run on seeds 1 and 2; presets v1)

The criterion was fixed in code before any run. The planted artefact is a gradient of 1x
the peak signal plus 8 blotches (amplitude 0.4x, sigma 8-30 px). Each experiment is
simulated with and without it under the same seed. A hit means the influence of
background + band definition rises by more than that of any other decision.

* Hit rate: 67 % (seed 1) and 63 % (seed 2), below the 80 % target. With the calibrated
  presets v2: 74 % and 65 %.
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

This matters for level 3: deposited "raw" images are often processed. Native imager files
(`.scn`, `.gel`) are reliable; [real_data.md](real_data.md) lists datasets that hold them.

## Open items

* **E1 and E5:** resolved by protocol v2, which was pre-specified and confirmed on seed 3.
  E1 is descriptive; E5 is the paired average criterion.
* **Known truth on real images:** Pillai-Kastoori et al. (2020) deposited raw LI-COR
  acquisitions of a 40–1.25 µg dilution series. They are being integrated as a
  real-image linearity check; see real_data.md.
* **Level 2:** a laboratory dilution and spike-in series (BlotVerse-Bench) is still needed
  for a full known-truth validation on real images.
