# BlotVerse level-1 validation

2000 simulated experiments (half null) + 200 with a planted background artefact (paired with and without it); 500 joint permutations each; seed 1; 123 s; 1 failed.

BlotVerse 0.1.0, spec sha256 `d332a71088623219838617815e3323d26fafb2f7db0b5ead4da9c32d0190d210`.

| Experiment | Metric | Value | Target | Result |
|---|---|---|---|---|
| E1 | median log10(p_max/p_min), real effects, moderate artefacts | 1.20 (26.6 % span >= 2 orders) | several orders (>= 2) | FAIL |
| E2 | AUC of R (real vs null), evaluable designs | 0.913 | >= 0.85 | PASS |
| E2' | AUC of R, all designs (reference) | 0.862 | - | - |
| E3 | nulls labelled Robust | 0.7 % | <= 5 % | PASS |
| E4 | joint-test type I error, evaluable designs | 0.038 | <= 0.05 | PASS |
| E4' | joint-test type I error, all designs (reference) | 0.023 | - | - |
| E5 | planted background artefact raises background + band influence most (paired) | 67.3 % | >= 80 % | FAIL |
| E5' | background is the single most influential decision (reference) | 18.1 % | - | - |
| E6 | rolling-ball branches vs Fiji | separate run | r >= 0.99 | see validation/results/e6_fiji |

Reference: type I error of the single default analysis = 0.041. 48.1 % of experiments had a design the joint test can evaluate.

**Decision point G2 (E2, E3, E4): PASS**

E1 by artefact level (real effects, median orders of magnitude): clean 0.99; moderate 1.20; severe 1.92; moderate nulls (reference) 0.46.

AUC by stratum:

| Stratum | AUC |
|---|---|
| level=clean | 0.874 |
| level=moderate | 0.866 |
| level=severe | 0.847 |
| |lfc|=0.3 | 0.715 |
| |lfc|=0.5 | 0.896 |
| |lfc|=1.0 | 0.969 |

Verdicts by design (gels x replicates per group per gel):

| Design | Truth | n | Robust | Fragile | Not supported | Not evaluable |
|---|---|---|---|---|---|---|
| 2x2 | null | 251 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x2 | real | 267 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 2x3 | null | 233 | 1.3 % | 0.9 % | 97.9 % | 0.0 % |
| 2x3 | real | 208 | 66.8 % | 10.1 % | 23.1 % | 0.0 % |
| 3x1 | null | 273 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x1 | real | 247 | 0.0 % | 0.0 % | 0.0 % | 100.0 % |
| 3x2 | null | 243 | 1.6 % | 1.2 % | 97.1 % | 0.0 % |
| 3x2 | real | 278 | 62.6 % | 9.0 % | 28.4 % | 0.0 % |
