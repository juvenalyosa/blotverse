# BlotVerse level-1 validation

2000 simulated experiments (half null) + 200 with a planted background gradient; 500 joint permutations each; seed 1; 229 s; 0 failed.

BlotVerse 0.1.0, spec sha256 `cfe7af63d43f2f1b9044b49a293b37bd9f0e99c35943a419821731205f647206`.

| Experiment | Metric | Value | Target | Result |
|---|---|---|---|---|
| E1 | median log10(p_max/p_min), moderate artefacts | 0.98 (22.2 % span >= 2 orders) | several orders | FAIL |
| E2 | AUC of R (real vs null) | 0.880 | >= 0.85 | PASS |
| E3 | nulls labelled Robust | 1.0 % | <= 5 % | PASS |
| E4 | joint-test type I error | 0.032 | <= 0.05 | PASS |
| E5 | background is most influential (gradient planted) | 36.0 % | >= 80 % | FAIL |
| E6 | default branch vs Fiji | not run here | r >= 0.99 | see validation/fiji |

Reference: type I error of the single default analysis = 0.041.

**Decision point G2 (E2, E3, E4): PASS**

AUC by stratum:

| Stratum | AUC |
|---|---|
| level=clean | 0.897 |
| level=moderate | 0.887 |
| level=severe | 0.859 |
| |lfc|=0.3 | 0.730 |
| |lfc|=0.5 | 0.923 |
| |lfc|=1.0 | 0.984 |
