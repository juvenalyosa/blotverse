# BlotVerse level-1 validation (protocol v2)

2000 simulated experiments (half null) + 200 with a planted background artefact (paired with and without it); 500 joint permutations each; seed 1; 141 s; 0 failed.

BlotVerse 0.1.0, spec sha256 `d332a71088623219838617815e3323d26fafb2f7db0b5ead4da9c32d0190d210`, simulator presets v2. Criteria: docs/validation_protocol.md. Intervals are 95 % (E5: 98.75 %, Bonferroni).

| Criterion | Estimate [CI] | n | Rule | Result |
|---|---|---|---|---|
| E2 AUC of R, evaluable designs | 0.918 [0.900, 0.935] | 962 | lower bound >= 0.85 | PASS |
| E3 evaluable nulls labelled Robust | 1.3 % [0.6 %, 2.7 %] | 476 | upper bound <= 5 % | PASS |
| E4 joint-test type I error, evaluable nulls | 0.042 [0.027, 0.064] | 476 | estimate <= 0.05 and lower bound <= 0.05 | PASS |
| E5 mean change in background + band influence (H) | 0.189 [0.131, 0.243] | 200 | lower bound > 0 | PASS |
| E5 margin H - change in loading | 0.537 [0.417, 0.649] | 200 | lower bound > 0 | PASS |
| E5 margin H - change in normalization | 0.194 [0.137, 0.249] | 200 | lower bound > 0 | PASS |
| E5 margin H - change in scale | 0.243 [0.182, 0.304] | 200 | lower bound > 0 | PASS |
| E5 margin H - change in test | 0.189 [0.131, 0.243] | 200 | lower bound > 0 | PASS |

**G2 (E2, E3, E4): PASS. E5: PASS.**

References (not decisive):

* AUC of R, all designs: 0.860; evaluable fraction 48.1 %.
* Joint-test type I error, all designs: 0.024; single default analysis: 0.040 [0.030, 0.054].
* E5 per-experiment hit rate: 74.0 % [67.5 %, 79.6 %]; background alone, mean change: 0.122 [0.100, 0.145]; background top decision (protocol v1): 19.5 %.
* E6 (agreement with Fiji): separate run, see validation/results/e6_fiji.

E1 (descriptive): spread of evidence across universes

| Level | Truth | D1 median orders of magnitude [CI] | D2 universes disagree on significance [CI] |
|---|---|---|---|
| clean | real | 0.99 [0.89, 1.16] | 21.4 % [17.4 %, 26.0 %] |
| clean | null | 0.30 [0.25, 0.33] | 12.1 % [8.9 %, 16.1 %] |
| moderate | real | 0.96 [0.85, 1.05] | 24.6 % [20.2 %, 29.4 %] |
| moderate | null | 0.35 [0.32, 0.39] | 12.2 % [9.1 %, 16.2 %] |
| severe | real | 1.62 [1.46, 1.73] | 56.2 % [50.8 %, 61.6 %] |
| severe | null | 0.67 [0.62, 0.72] | 19.6 % [15.8 %, 24.0 %] |

AUC by stratum (all designs):

| Stratum | AUC |
|---|---|
| level=clean | 0.874 |
| level=moderate | 0.860 |
| level=severe | 0.845 |
| |lfc|=0.3 | 0.712 |
| |lfc|=0.5 | 0.891 |
| |lfc|=1 | 0.971 |

Verdicts by design (gels x replicates per group per gel):

| Design | Truth | n | Robust | Fragile | Not supported | Not evaluable |
|---|---|---|---|---|---|---|
| 2x2 | null | 251 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x2 | real | 267 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x3 | null | 233 | 1.3 % | 0.9 % | 97.9 % | 0.0 % |
| 2x3 | real | 208 | 69.7 % | 5.8 % | 24.5 % | 0.0 % |
| 3x1 | null | 273 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x1 | real | 247 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x2 | null | 243 | 1.2 % | 2.5 % | 96.3 % | 0.0 % |
| 3x2 | real | 278 | 67.6 % | 5.8 % | 26.6 % | 0.0 % |
