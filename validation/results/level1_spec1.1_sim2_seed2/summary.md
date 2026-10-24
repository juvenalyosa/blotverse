# BlotVerse level-1 validation

2000 simulated experiments (half null) + 200 with a planted background artefact (paired with and without it); 500 joint permutations each; seed 2; 133 s; 1 failed.

BlotVerse 0.1.0, spec sha256 `d332a71088623219838617815e3323d26fafb2f7db0b5ead4da9c32d0190d210`, simulator presets v2.

| Experiment | Metric | Value | Target | Result |
|---|---|---|---|---|
| E1 | median log10(p_max/p_min), real effects, moderate artefacts | 1.10 (26.5 % span >= 2 orders) | several orders (>= 2) | FAIL |
| E2 | AUC of R (real vs null), evaluable designs | 0.905 | >= 0.85 | PASS |
| E2' | AUC of R, all designs (reference) | 0.845 | - | - |
| E3 | nulls labelled Robust | 0.7 % | <= 5 % | PASS |
| E4 | joint-test type I error, evaluable designs | 0.042 | <= 0.05 | PASS |
| E4' | joint-test type I error, all designs (reference) | 0.029 | - | - |
| E5 | planted background artefact raises background + band influence most (paired) | 65.3 % | >= 80 % | FAIL |
| E5' | background is the single most influential decision (reference) | 13.1 % | - | - |
| E6 | rolling-ball branches vs Fiji | separate run | r >= 0.99 | see validation/results/e6_fiji |

Reference: type I error of the single default analysis = 0.051. 49.5 % of experiments had a design the joint test can evaluate.

**Decision point G2 (E2, E3, E4): PASS**

E1 by artefact level (real effects, median orders of magnitude): clean 0.92; moderate 1.10; severe 1.50; moderate nulls (reference) 0.35.

AUC by stratum:

| Stratum | AUC |
|---|---|
| level=clean | 0.849 |
| level=moderate | 0.853 |
| level=severe | 0.835 |
| |lfc|=0.3 | 0.693 |
| |lfc|=0.5 | 0.872 |
| |lfc|=1.0 | 0.978 |

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
