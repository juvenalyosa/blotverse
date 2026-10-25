# BlotVerse level-1 validation (protocol v2)

2000 simulated experiments (half null) + 200 with a planted background artefact (paired with and without it); 500 joint permutations each; seed 2; 130 s; 1 failed.

BlotVerse 0.1.0, spec sha256 `d332a71088623219838617815e3323d26fafb2f7db0b5ead4da9c32d0190d210`, simulator presets v2. Criteria: docs/validation_protocol.md. Intervals are 95 % (E5: 98.75 %, Bonferroni).

| Criterion | Estimate [CI] | n | Rule | Result |
|---|---|---|---|---|
| E2 AUC of R, evaluable designs | 0.905 [0.887, 0.924] | 991 | lower bound >= 0.85 | PASS |
| E3 evaluable nulls labelled Robust | 1.4 % [0.7 %, 2.9 %] | 495 | upper bound <= 5 % | PASS |
| E4 joint-test type I error, evaluable nulls | 0.042 [0.028, 0.064] | 495 | estimate <= 0.05 and lower bound <= 0.05 | PASS |
| E5 mean change in background + band influence (H) | 0.154 [0.095, 0.215] | 199 | lower bound > 0 | PASS |
| E5 margin H - change in loading | 0.416 [0.294, 0.534] | 199 | lower bound > 0 | PASS |
| E5 margin H - change in normalization | 0.162 [0.102, 0.223] | 199 | lower bound > 0 | PASS |
| E5 margin H - change in scale | 0.216 [0.144, 0.286] | 199 | lower bound > 0 | PASS |
| E5 margin H - change in test | 0.154 [0.095, 0.215] | 199 | lower bound > 0 | PASS |

**G2 (E2, E3, E4): PASS. E5: PASS.**

References (not decisive):

* AUC of R, all designs: 0.845; evaluable fraction 49.5 %.
* Joint-test type I error, all designs: 0.029; single default analysis: 0.051 [0.039, 0.066].
* E5 per-experiment hit rate: 65.3 % [58.5 %, 71.6 %]; background alone, mean change: 0.102 [0.083, 0.123]; background top decision (protocol v1): 13.1 %.
* E6 (agreement with Fiji): separate run, see validation/results/e6_fiji.

E1 (descriptive): spread of evidence across universes

| Level | Truth | D1 median orders of magnitude [CI] | D2 universes disagree on significance [CI] |
|---|---|---|---|
| clean | real | 0.92 [0.79, 1.03] | 17.5 % [13.8 %, 21.9 %] |
| clean | null | 0.29 [0.26, 0.32] | 9.9 % [7.2 %, 13.5 %] |
| moderate | real | 1.10 [0.97, 1.28] | 21.2 % [17.2 %, 25.9 %] |
| moderate | null | 0.35 [0.32, 0.39] | 13.7 % [10.3 %, 17.8 %] |
| severe | real | 1.50 [1.42, 1.62] | 56.2 % [50.7 %, 61.5 %] |
| severe | null | 0.72 [0.67, 0.78] | 18.8 % [15.0 %, 23.3 %] |

AUC by stratum (all designs):

| Stratum | AUC |
|---|---|
| level=clean | 0.849 |
| level=moderate | 0.853 |
| level=severe | 0.835 |
| |lfc|=0.3 | 0.693 |
| |lfc|=0.5 | 0.872 |
| |lfc|=1 | 0.978 |

Verdicts by design (gels x replicates per group per gel):

| Design | Truth | n | Robust | Fragile | Not supported | Not evaluable |
|---|---|---|---|---|---|---|
| 2x2 | null | 255 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x2 | real | 255 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x3 | null | 258 | 2.3 % | 3.1 % | 94.6 % | 0.0 % |
| 2x3 | real | 234 | 64.1 % | 7.3 % | 28.6 % | 0.0 % |
| 3x1 | null | 250 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x1 | real | 249 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x2 | null | 237 | 0.4 % | 1.7 % | 97.9 % | 0.0 % |
| 3x2 | real | 262 | 60.7 % | 6.1 % | 33.2 % | 0.0 % |

Failures:

* background #58 (moderate): blot 'gel2': lane detection found no lane signal; supply a manual annotation
