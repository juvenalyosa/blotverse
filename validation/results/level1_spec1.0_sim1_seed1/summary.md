# BlotVerse level-1 validation

2000 simulated experiments (half null) + 200 with a planted background artefact (paired with and without it); 500 joint permutations each; seed 1; 229 s; 0 failed.

BlotVerse 0.1.0, spec sha256 `cfe7af63d43f2f1b9044b49a293b37bd9f0e99c35943a419821731205f647206`, simulator presets v1.

| Experiment | Metric | Value | Target | Result |
|---|---|---|---|---|
| E1 | median log10(p_max/p_min), real effects, moderate artefacts | 1.70 (39.8 % span >= 2 orders) | several orders (>= 2) | FAIL |
| E2 | AUC of R (real vs null), evaluable designs | 0.880 | >= 0.85 | PASS |
| E2' | AUC of R, all designs (reference) | 0.880 | - | - |
| E3 | nulls labelled Robust | 1.0 % | <= 5 % | PASS |
| E4 | joint-test type I error, evaluable designs | 0.032 | <= 0.05 | PASS |
| E4' | joint-test type I error, all designs (reference) | 0.032 | - | - |
| E5 | planted background artefact raises background + band influence most (paired) | 36.0 % | >= 80 % | FAIL |
| E5' | background is the single most influential decision (reference) | 36.0 % | - | - |
| E6 | rolling-ball branches vs Fiji | separate run | r >= 0.99 | see validation/results/e6_fiji |

Reference: type I error of the single default analysis = 0.041. 100.0 % of experiments had a design the joint test can evaluate.

**Decision point G2 (E2, E3, E4): PASS**

E1 by artefact level (real effects, median orders of magnitude): clean 1.42; moderate 1.70; severe 2.23; moderate nulls (reference) 0.54.

AUC by stratum:

| Stratum | AUC |
|---|---|
| level=clean | 0.897 |
| level=moderate | 0.887 |
| level=severe | 0.859 |
| |lfc|=0.3 | 0.730 |
| |lfc|=0.5 | 0.923 |
| |lfc|=1.0 | 0.984 |

Verdicts by design (gels x replicates per group per gel):

| Design | Truth | n | Robust | Fragile | Not supported | Not evaluable |
|---|---|---|---|---|---|---|
| 2x2 | null | 251 | 1.2 % | 0.8 % | 98.0 % | 0.0 % |
| 2x2 | real | 267 | 35.6 % | 20.2 % | 44.2 % | 0.0 % |
| 2x3 | null | 233 | 1.3 % | 1.3 % | 97.4 % | 0.0 % |
| 2x3 | real | 208 | 67.3 % | 9.6 % | 23.1 % | 0.0 % |
| 3x1 | null | 273 | 0.0 % | 0.0 % | 100.0 % | 0.0 % |
| 3x1 | real | 247 | 0.0 % | 0.0 % | 100.0 % | 0.0 % |
| 3x2 | null | 243 | 1.6 % | 1.6 % | 96.7 % | 0.0 % |
| 3x2 | real | 278 | 62.2 % | 9.4 % | 28.4 % | 0.0 % |
