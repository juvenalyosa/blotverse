# BlotVerse level-1 validation

2000 simulated experiments (half null) + 200 with a planted background gradient; 500 joint permutations each; seed 2; 117 s; 0 failed.

BlotVerse 0.1.0, spec sha256 `d332a71088623219838617815e3323d26fafb2f7db0b5ead4da9c32d0190d210`.

| Experiment | Metric | Value | Target | Result |
|---|---|---|---|---|
| E1 | median log10(p_max/p_min), real effects, moderate artefacts | 1.37 (31.6 % span >= 2 orders) | several orders (>= 2) | FAIL |
| E2 | AUC of R (real vs null), evaluable designs | 0.907 | >= 0.85 | PASS |
| E2' | AUC of R, all designs (reference) | 0.846 | - | - |
| E3 | nulls labelled Robust | 0.5 % | <= 5 % | PASS |
| E4 | joint-test type I error, evaluable designs | 0.034 | <= 0.05 | PASS |
| E4' | joint-test type I error, all designs (reference) | 0.023 | - | - |
| E5 | background is most influential (gradient planted) | 40.0 % | >= 80 % | FAIL |
| E6 | default branch vs Fiji | not run here | r >= 0.99 | see validation/fiji |

Reference: type I error of the single default analysis = 0.056. 49.5 % of experiments had a design the joint test can evaluate.

**Decision point G2 (E2, E3, E4): PASS**

E1 by artefact level (real effects, median orders of magnitude): clean 0.92; moderate 1.37; severe 1.96; moderate nulls (reference) 0.44.

AUC by stratum:

| Stratum | AUC |
|---|---|
| level=clean | 0.849 |
| level=moderate | 0.865 |
| level=severe | 0.827 |
| |lfc|=0.3 | 0.691 |
| |lfc|=0.5 | 0.873 |
| |lfc|=1.0 | 0.983 |

Verdicts by design (gels x replicates per group per gel):

| Design | Truth | n | Robust | Fragile | Not supported | Not evaluable |
|---|---|---|---|---|---|---|
| 2x2 | null | 255 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x2 | real | 255 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x3 | null | 258 | 1.6 % | 2.3 % | 96.1 % | 0.0 % |
| 2x3 | real | 234 | 59.0 % | 10.3 % | 30.8 % | 0.0 % |
| 3x1 | null | 250 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x1 | real | 249 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x2 | null | 237 | 0.4 % | 0.8 % | 98.7 % | 0.0 % |
| 3x2 | real | 262 | 58.0 % | 7.3 % | 34.7 % | 0.0 % |
