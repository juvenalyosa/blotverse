# Level-1 validation results (simulation with known truth)

Command (about 4 minutes on a 10-core laptop):

```bash
build/apps/blotverse validate --out validation/results/level1 \
  --experiments 2000 --gradient-experiments 200 --permutations 500 --seed 1
```

* BlotVerse 0.1.0, spec sha256 `cfe7af63d43f2f1b9044b49a293b37bd9f0e99c35943a419821731205f647206`.
* 2000 simulated experiments (exactly half null; true |log2 FC| of 0.3, 0.5 or 1.0 with
  random sign), clean / moderate / severe artefacts, designs of 3 gels x 1, 2 x 2,
  3 x 2 and 2 x 3 replicates per group.
* Plus 200 experiments with a planted background gradient (E5).
* Every experiment goes through the full tool: automatic detection, QC, 432 universes and
  a joint test with 500 within-blot permutations. 0 of 2200 experiments failed.
* Raw per-experiment data: [`validation/results/level1/records.csv`](../validation/results/level1/records.csv).

## Results

| Exp. | Question | Value | Target | Result |
|---|---|---|---|---|
| E1 | Spread of p-values across universes (moderate artefacts) | median 0.98 orders of magnitude; 22 % of experiments span >= 2 orders | several orders | **not met** |
| E2 | Does R separate real effects from nulls? (AUC) | **0.880** | >= 0.85 | met |
| E3 | Nulls labelled "Robust" | **1.0 %** | <= 5 % | met |
| E4 | Type I error of the joint test | **0.032** | <= 0.05 | met |
| E5 | Planted gradient: is background the most influential decision? | 36 % | >= 80 % | **not met** |
| E6 | Default branch vs Fiji (r) | not run (Fiji not installed) | >= 0.99 | pending |

**Decision point G2 (E2 + E3 + E4): met.** For reference, the type I error of the single
default analysis is 0.041 (1000 nulls), so the default test is well calibrated.

AUC by stratum: clean 0.897, moderate 0.887, severe 0.861; |lfc| = 0.3: 0.730,
0.5: 0.923, 1.0: 0.986.

G1 (speed): one 3-blot experiment with 432 universes and 1000 joint permutations takes
about 0.3 s (target < 2 min).

## Findings that need a decision before freezing the spec

1. **E1 is below target.** With the current "moderate" simulator preset the spread of
   p-values is about one order of magnitude, far from the Gassmann (2009) range. Either
   the simulator is milder than real blots or the branch set is more homogeneous than the
   procedures Gassmann compared. The realism check against public raw images (plan
   section 8) should settle this. The simulator was *not* tuned to hit the target.
2. **E5 is below target.** A strong planted gradient raises the influence of background
   subtraction from eta^2 = 0.07 to 0.18, but the loading control (eta^2 = 0.27) stays
   the most influential decision in most experiments. All four background branches
   remove smooth gradients well, so the gradient barely separates them.
3. **The joint test has a design floor.** Labels are permuted within blots. With
   3 gels x 1 replicate per group there are only 8 relabellings, so p < 0.05 is
   impossible: 0 % of real effects can be "Robust" in that design (mean R = 0.36). The
   report states this when it happens. Robust rate for real effects by design:
   3x1 0 %, 2x2 36 %, 3x2 62 %, 2x3 67 %.
4. **The permutation-test branch cannot reach p < 0.05 with 3 replicates per group**
   (20 relabellings, smallest two-sided p = 0.1), so it always counts against R in
   those designs.
5. **Blot-sum and optimal-alignment normalisation are anti-conservative with one
   control and one treatment lane per blot.** Per-branch null diagnostic: Welch type I
   error 0.17 (sum) and 0.15 (alignment), because dividing by the blot total makes
   the two lanes negatively correlated. The default (fixed point) stays at 0.048.

Items 3 to 5 are consequences of the specification, not code defects. Whether to
restrict designs, adjust the branches or keep them is a pre-registration decision.
