# Exposure-invariance bench: pooled results

4 series (plcg_ip_2025_flag_left, plcg_ip_2025_flag_right, plcg_ip_2025_plc, set1ip_2026_gfp); default branch `rb2/profile`; distortion threshold 0.2 log2.

## P1. QC misses

3 of 32 usable exposure x band combinations (3 carry a saturation flag).

* plcg_ip_2025_plc band r404 exposure 2 (plc_20s.tif)
* plcg_ip_2025_plc band r444 exposure 2 (plc_20s.tif)
* set1ip_2026_gfp band doublet exposure 2 (gfp_295s.tif)

Not evaluable (a lane saturated at every exposure, so no reference): 3 bands: plcg_ip_2025_flag_left band r248; plcg_ip_2025_flag_left band r278; set1ip_2026_gfp band gfp.

## P2. Compressive-distorted pairs with a flagged lane (default branch)

9 / 20 = 0.450 (Wilson 95 % 0.258-0.658).

## S1-S3. Per branch

| Branch | Clean pairs | median abs delta | q95 abs delta | Clean pairs > 0.2 | Flagged pairs | median abs delta (flagged) |
|---|---|---|---|---|---|---|
| local/mask | 82 | 0.060 | 0.317 | 16 | 10 | 0.171 |
| local/profile | 82 | 0.052 | 0.383 | 11 | 10 | 0.749 |
| local/rect | 82 | 0.057 | 0.429 | 13 | 10 | 0.417 |
| rb1/mask | 82 | 0.056 | 0.256 | 11 | 10 | 0.061 |
| rb1/profile | 82 | 0.059 | 0.451 | 14 | 10 | 0.751 |
| rb1/rect | 82 | 0.060 | 0.238 | 6 | 10 | 0.328 |
| rb2/mask | 82 | 0.080 | 0.617 | 16 | 10 | 0.122 |
| rb2/profile | 82 | 0.065 | 0.476 | 13 | 10 | 0.749 |
| rb2/rect | 82 | 0.044 | 0.213 | 6 | 10 | 0.256 |
| rb4/mask | 82 | 0.069 | 0.396 | 14 | 10 | 0.160 |
| rb4/profile | 82 | 0.069 | 0.422 | 14 | 10 | 0.749 |
| rb4/rect | 82 | 0.043 | 0.255 | 10 | 10 | 0.230 |
