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
* 200 experiments with a planted background gradient (E5).
* The full tool on every experiment: automatic detection, QC, the multiverse and the
  joint test. 0 of 2200 experiments failed in every run.

## Current results: spec v1.1.0 (sha256 `d332a710…d210`)

| Exp. | Question | seed 1 | seed 2 (new data) | Target | Result |
|---|---|---|---|---|---|
| E1 | p-value spread across universes, moderate artefacts (median orders of magnitude) | 0.74 | 0.72 | several orders | **not met** |
| E2 | AUC of R, real vs null (evaluable designs) | **0.913** | **0.907** | >= 0.85 | met |
| E2' | AUC of R, all designs (reference) | 0.862 | 0.846 | – | – |
| E3 | Nulls labelled "Robust" | **0.7 %** | **0.5 %** | <= 5 % | met |
| E4 | Joint-test type I error (evaluable designs) | **0.038** | **0.034** | <= 0.05 | met |
| E4' | Joint-test type I error, all designs (reference) | 0.023 | 0.023 | – | – |
| E5 | Planted gradient → background most influential | 35 % | 40 % | >= 80 % | **not met** |
| E6 | Default branch vs Fiji (r) | – | – | >= 0.99 | pending (needs Fiji) |

**G2 (E2 + E3 + E4): met for both seeds.** The type I error of the single default analysis
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

## Open items

* **E1** (spread of p-values) is far below the range reported by Gassmann (2009). The
  simulator was not tuned to hit this target. The realism check against public raw images
  (plan section 8) should decide whether the simulator is too mild or the branch set too
  homogeneous.
* **E5:** a strong planted gradient raises the influence of background subtraction from
  eta^2 = 0.07 to 0.18, but the loading control (eta^2 = 0.27) usually stays the most
  influential decision, because all four background branches remove smooth gradients well.
  Possible redesigns, such as a branch without background subtraction or a criterion based
  on the *change* in influence, should be decided before seeing new results.
* **E6** needs Fiji: run `validation/fiji/run_e6.sh WORK_DIR /path/to/ImageJ-macosx`.
