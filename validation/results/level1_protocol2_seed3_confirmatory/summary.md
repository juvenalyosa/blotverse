# BlotVerse level-1 validation (protocol v2)

6000 simulated experiments (half null) + 600 with a planted background artefact (paired with and without it); 500 joint permutations each; seed 3; 445 s; 0 failed.

BlotVerse 0.1.0, spec sha256 `d332a71088623219838617815e3323d26fafb2f7db0b5ead4da9c32d0190d210`, simulator presets v2. Criteria: docs/validation_protocol.md. Intervals are 95 % (E5: 98.75 %, Bonferroni).

| Criterion | Estimate [CI] | n | Rule | Result |
|---|---|---|---|---|
| E2 AUC of R, evaluable designs | 0.925 [0.915, 0.934] | 3028 | lower bound >= 0.85 | PASS |
| E3 evaluable nulls labelled Robust | 1.8 % [1.2 %, 2.6 %] | 1523 | upper bound <= 5 % | PASS |
| E4 joint-test type I error, evaluable nulls | 0.033 [0.026, 0.044] | 1523 | estimate <= 0.05 and lower bound <= 0.05 | PASS |
| E5 mean change in background + band influence (H) | 0.179 [0.148, 0.210] | 600 | lower bound > 0 | PASS |
| E5 margin H - change in loading | 0.508 [0.448, 0.572] | 600 | lower bound > 0 | PASS |
| E5 margin H - change in normalization | 0.189 [0.158, 0.220] | 600 | lower bound > 0 | PASS |
| E5 margin H - change in scale | 0.233 [0.199, 0.270] | 600 | lower bound > 0 | PASS |
| E5 margin H - change in test | 0.179 [0.148, 0.210] | 600 | lower bound > 0 | PASS |

**G2 (E2, E3, E4): PASS. E5: PASS.**

References (not decisive):

* AUC of R, all designs: 0.864; evaluable fraction 50.5 %.
* Joint-test type I error, all designs: 0.020; single default analysis: 0.036 [0.030, 0.043].
* E5 per-experiment hit rate: 72.3 % [68.6 %, 75.8 %]; background alone, mean change: 0.126 [0.112, 0.138]; background top decision (protocol v1): 19.3 %.
* E6 (agreement with Fiji): separate run, see validation/results/e6_fiji.

E1 (descriptive): spread of evidence across universes

| Level | Truth | D1 median orders of magnitude [CI] | D2 universes disagree on significance [CI] |
|---|---|---|---|
| clean | real | 0.93 [0.88, 1.01] | 23.5 % [21.0 %, 26.2 %] |
| clean | null | 0.28 [0.27, 0.30] | 9.1 % [7.5 %, 11.0 %] |
| moderate | real | 1.10 [1.02, 1.20] | 23.8 % [21.2 %, 26.5 %] |
| moderate | null | 0.34 [0.33, 0.36] | 10.0 % [8.3 %, 12.1 %] |
| severe | real | 1.54 [1.45, 1.59] | 58.6 % [55.6 %, 61.6 %] |
| severe | null | 0.65 [0.62, 0.69] | 18.1 % [15.8 %, 20.7 %] |

AUC by stratum (all designs):

| Stratum | AUC |
|---|---|
| level=clean | 0.874 |
| level=moderate | 0.871 |
| level=severe | 0.850 |
| |lfc|=0.3 | 0.739 |
| |lfc|=0.5 | 0.884 |
| |lfc|=1 | 0.972 |

Verdicts by design (gels x replicates per group per gel):

| Design | Truth | n | Robust | Fragile | Not supported | Not evaluable |
|---|---|---|---|---|---|---|
| 2x2 | null | 758 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x2 | real | 718 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x3 | null | 755 | 1.9 % | 0.9 % | 97.2 % | 0.0 % |
| 2x3 | real | 760 | 68.2 % | 10.0 % | 21.8 % | 0.0 % |
| 3x1 | null | 719 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x1 | real | 777 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x2 | null | 768 | 1.7 % | 1.4 % | 96.9 % | 0.0 % |
| 3x2 | real | 745 | 64.2 % | 8.5 % | 27.4 % | 0.0 % |
