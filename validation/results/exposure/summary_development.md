# Exposure-invariance bench: pooled results

2 series (gtsf1_2026_series_a, gtsf1_2026_series_b); default branch `rb2/profile`; distortion threshold 0.2 log2.

## P1. QC misses

1 of 20 usable exposure x band combinations (8 carry a saturation flag).

* gtsf1_2026_series_b band lower exposure 5 (anti_GTSF1_150sec.scn)

Not evaluable (a lane saturated at every exposure, so no reference): 0 bands.

## P2. Compressive-distorted pairs with a flagged lane (default branch)

92 / 94 = 0.979 (Wilson 95 % 0.926-0.994).

## S1-S3. Per branch

| Branch | Clean pairs | median abs delta | q95 abs delta | Clean pairs > 0.2 | Flagged pairs | median abs delta (flagged) |
|---|---|---|---|---|---|---|
| local/mask | 27 | 0.038 | 0.387 | 3 | 100 | 0.884 |
| local/profile | 27 | 0.015 | 0.503 | 3 | 100 | 2.171 |
| local/rect | 27 | 0.022 | 0.405 | 3 | 100 | 0.961 |
| rb1/mask | 27 | 0.019 | 0.331 | 3 | 100 | 0.713 |
| rb1/profile | 27 | 0.044 | 0.527 | 5 | 100 | 2.171 |
| rb1/rect | 27 | 0.014 | 0.393 | 3 | 100 | 0.673 |
| rb2/mask | 27 | 0.042 | 0.362 | 5 | 100 | 0.537 |
| rb2/profile | 27 | 0.029 | 1.050 | 5 | 100 | 2.171 |
| rb2/rect | 27 | 0.022 | 0.443 | 3 | 100 | 0.583 |
| rb4/mask | 27 | 0.020 | 0.249 | 3 | 100 | 0.504 |
| rb4/profile | 27 | 0.011 | 0.441 | 3 | 100 | 2.171 |
| rb4/rect | 27 | 0.020 | 0.069 | 0 | 100 | 0.569 |
