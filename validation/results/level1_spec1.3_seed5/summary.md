# BlotVerse level-1 validation (protocol v2)

6000 simulated experiments (half null) + 600 with a planted background artefact (paired with and without it); 500 joint permutations each; seed 5; 421 s; 0 failed.

BlotVerse 0.1.0, spec sha256 `b13c69ec862eec1610bdab912bd96b512c88ca4cf8cc110492e1c1db72185d24`, simulator presets v2. Criteria: docs/validation_protocol.md. Intervals are 95 % (E5: 98.75 %, Bonferroni).

| Criterion | Estimate [CI] | n | Rule | Result |
|---|---|---|---|---|
| E2 AUC of R, evaluable designs | 0.923 [0.914, 0.933] | 3049 | lower bound >= 0.85 | PASS |
| E3 evaluable nulls labelled Robust | 1.9 % [1.4 %, 2.8 %] | 1539 | upper bound <= 5 % | PASS |
| E4 joint-test type I error, evaluable nulls | 0.032 [0.025, 0.043] | 1539 | estimate <= 0.05 and lower bound <= 0.05 | PASS |
| E5 mean change in background + band influence (H) | 0.206 [0.172, 0.237] | 600 | lower bound > 0 | PASS |
| E5 margin H - change in loading | 0.559 [0.493, 0.623] | 600 | lower bound > 0 | PASS |
| E5 margin H - change in normalization | 0.212 [0.178, 0.243] | 600 | lower bound > 0 | PASS |
| E5 margin H - change in scale | 0.257 [0.220, 0.294] | 600 | lower bound > 0 | PASS |
| E5 margin H - change in test | 0.206 [0.172, 0.237] | 600 | lower bound > 0 | PASS |

**G2 (E2, E3, E4): PASS. E5: PASS.**

References (not decisive):

* AUC of R, all designs: 0.867; evaluable fraction 50.8 %.
* Joint-test type I error, all designs: 0.021; single default analysis: 0.045 [0.038, 0.053].
* E5 per-experiment hit rate: 72.0 % [68.3 %, 75.4 %]; background alone, mean change: 0.118 [0.106, 0.131]; background top decision (protocol v1): 17.7 %.
* E6 (agreement with Fiji): separate run, see validation/results/e6_fiji.

E1 (descriptive): spread of evidence across universes

| Level | Truth | D1 median orders of magnitude [CI] | D2 universes disagree on significance [CI] |
|---|---|---|---|
| clean | real | 0.89 [0.82, 0.97] | 22.4 % [19.9 %, 25.1 %] |
| clean | null | 0.28 [0.27, 0.30] | 8.5 % [6.9 %, 10.4 %] |
| moderate | real | 1.11 [1.03, 1.19] | 24.2 % [21.7 %, 27.0 %] |
| moderate | null | 0.37 [0.36, 0.39] | 14.1 % [12.1 %, 16.4 %] |
| severe | real | 1.60 [1.53, 1.71] | 57.5 % [54.4 %, 60.5 %] |
| severe | null | 0.67 [0.63, 0.70] | 18.8 % [16.5 %, 21.3 %] |

AUC by stratum (all designs):

| Stratum | AUC |
|---|---|
| level=clean | 0.871 |
| level=moderate | 0.875 |
| level=severe | 0.857 |
| |lfc|=0.3 | 0.743 |
| |lfc|=0.5 | 0.880 |
| |lfc|=1 | 0.973 |

Verdicts by design (gels x replicates per group per gel):

| Design | Truth | n | Robust | Fragile | Not supported | Not evaluable |
|---|---|---|---|---|---|---|
| 2x2 | null | 736 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x2 | real | 724 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x3 | null | 755 | 2.3 % | 0.7 % | 97.1 % | 0.0 % |
| 2x3 | real | 737 | 66.8 % | 8.0 % | 25.2 % | 0.0 % |
| 3x1 | null | 725 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x1 | real | 766 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x2 | null | 784 | 1.7 % | 1.4 % | 96.9 % | 0.0 % |
| 3x2 | real | 773 | 64.7 % | 8.0 % | 27.3 % | 0.0 % |
