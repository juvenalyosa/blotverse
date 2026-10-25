# Level-1 validation protocol, version 2 (pre-specified)

Status: **fixed before the confirmatory run.** This file and the code that implements it
were committed before seed 3 was run. Seeds 1 and 2 were already analysed under earlier
definitions, so they are **exploratory**. Seed 3 is **confirmatory**.

Why version 2: version 1 (plan section 8) used point estimates only, and two criteria (E1,
E5) turned out to be ill-posed (see validation.md). Version 2 makes every estimand,
interval and decision rule explicit.

## Runs

| Run | Role | Main experiments | E5 paired experiments | Joint permutations |
|---|---|---|---|---|
| seed 1, seed 2 | exploratory | 2000 | 200 | 500 |
| **seed 3** | **confirmatory** | **6000** | **600** | 500 |

The other settings are those of the current code:

* spec v1.1.0 (sha256 `d332a710…d210`);
* simulator presets v2;
* designs 3x1, 2x2, 3x2 and 2x3;
* half of the main experiments null; the rest with |lfc| in {0.3, 0.5, 1.0} and random sign;
* artefact levels drawn uniformly from clean, moderate and severe.

## Populations

* *Main*: main-set experiments that did not fail. Failures are reported with their cause.
* *Evaluable*: main experiments whose verdict is not "Not evaluable", i.e. whose design
  allows the joint test to reach alpha (2 / relabellings < alpha).
* *Real* / *null*: true lfc ≠ 0 / = 0.

## Interval methods

* **Proportions:** Wilson score interval, 95 %.
* **Medians and AUC:** percentile bootstrap, 2000 resamples. The seed is derived from the
  run seed, so intervals are reproducible. For AUC, real and null experiments are resampled
  separately.
* **E5 (four simultaneous comparisons):** Bonferroni-adjusted 98.75 % percentile bootstrap
  intervals of the paired mean differences, 2000 resamples.

## Criteria

| Id | Estimand | Decision rule |
|---|---|---|
| **E2** | AUC of R separating real from null, evaluable designs | **met if the lower 95 % bound ≥ 0.85** |
| **E3** | proportion of evaluable nulls labelled Robust | **met if the upper 95 % bound ≤ 0.05** |
| **E4** | type I error of the joint test (joint p < 0.05) among evaluable nulls | **met if the point estimate ≤ 0.05 and the lower 95 % bound ≤ 0.05** |
| **E5** | see below | **met if every bound below holds** |
| **E1** | see below | descriptive, no decision rule |

**G2 = E2 ∧ E3 ∧ E4.**

**Why E4 uses the lower bound.** A correctly calibrated test has a type I error of exactly
alpha. Requiring the upper bound to be ≤ 0.05 would demand a conservative test. The rule
therefore fails only when the error is detectably above alpha.

**E5: paired diagnostic.** Each artefact experiment is simulated twice with the same seed:

* once with the planted background artefact: gradient 1.0x peak plus 8 blotches,
  amplitude 0.4x peak, sigma 8–30 px;
* once without it.

Definitions:

* Δ_d = eta²_d(planted) − eta²_d(baseline), for each decision d.
* H = Δ_background + Δ_band. Background handling lives in both decisions: profile-peak and
  threshold-mask band definitions correct the local background themselves.
* For each other decision o in {loading, normalisation, scale, test}: C_o = H − Δ_o.

E5 is met if the lower 98.75 % bound of mean(H) is > 0 and, for every o, the lower 98.75 %
bound of mean(C_o) is > 0. In words: on average the planted background artefact raises the
influence of the background-handling decisions, and by more than it raises any other
decision's influence.

Reported but not decisive:

* the per-experiment hit rate (H > 0 and H > max_o Δ_o), with its Wilson interval;
* the same analysis for Δ_background alone.

**E1: descriptive (no pass/fail).** Gassmann et al. (2009) showed that the choice of
procedure can change a conclusion. For real effects, by artefact level:

* **D1:** median of log10(p_max / p_min) across valid universes, with a bootstrap 95 %
  interval;
* **D2:** proportion of experiments whose universes **disagree on significance** (at least
  one p < 0.05 and at least one p ≥ 0.05), with a Wilson 95 % interval. This is the
  conclusion-level analogue of Gassmann's finding.

D1 and D2 are also reported for nulls, as a reference.

## What changed from version 1

* **E2, E3, E4:** same thresholds as the plan, now judged by confidence bounds rather than
  point estimates.
* **E5:** the per-experiment "background is the top decision" rule is replaced by the paired
  average criterion above.
* **E1:** reported descriptively.

The version-1 numbers stay in validation.md.
