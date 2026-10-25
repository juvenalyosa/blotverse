# Exposure-invariance bench: pooled results

6 series (gtsf1_2026_series_a, gtsf1_2026_series_b, plcg_ip_2025_flag_left, plcg_ip_2025_flag_right, plcg_ip_2025_plc, set1ip_2026_gfp); default branch `rb2/profile`; distortion threshold 0.2 log2.

## P1. QC misses (v1.2 lane-level rule)

52 usable exposure x band combinations; 11 carry a saturation flag, 15 a saturation or near-saturation flag.

* **Spec rule (judged):** 0 misses
* Candidate near-saturation rule: 0 misses
* v1.1 pair rule, for comparison: 4 misses
  * gtsf1_2026_series_b band lower exposure 5 (anti_GTSF1_150sec.scn)
  * plcg_ip_2025_plc band r404 exposure 2 (plc_20s.tif)
  * plcg_ip_2025_plc band r444 exposure 2 (plc_20s.tif)
  * set1ip_2026_gfp band doublet exposure 2 (gfp_295s.tif)

Not evaluable (a lane saturated at every exposure, so no reference): 3 bands: plcg_ip_2025_flag_left band r248; plcg_ip_2025_flag_left band r278; set1ip_2026_gfp band gfp.

S4. Exposures blamed for low-signal distortion: 3
  * gtsf1_2026_series_b band lower exposure 1 (anti_GTSF1_5secauto.scn)
  * set1ip_2026_gfp band r188 exposure 1 (gfp_20s.tif)
  * set1ip_2026_gfp band doublet exposure 1 (gfp_20s.tif)

## P2. Compressive-distorted pairs with a flagged lane (default branch)

101 / 114 = 0.886 (Wilson 95 % 0.815-0.932).

## S1-S3. Per branch

| Branch | Clean pairs | median abs delta | q95 abs delta | Clean pairs > 0.2 | Flagged pairs | median abs delta (flagged) |
|---|---|---|---|---|---|---|
| local/mask | 109 | 0.051 | 0.362 | 19 | 110 | 0.730 |
| local/profile | 109 | 0.046 | 0.405 | 14 | 110 | 1.916 |
| local/rect | 109 | 0.046 | 0.423 | 16 | 110 | 0.834 |
| rb1/mask | 109 | 0.046 | 0.259 | 14 | 110 | 0.598 |
| rb1/profile | 109 | 0.051 | 0.492 | 19 | 110 | 1.917 |
| rb1/rect | 109 | 0.049 | 0.251 | 9 | 110 | 0.601 |
| rb2/mask | 109 | 0.073 | 0.590 | 21 | 110 | 0.469 |
| rb2/profile | 109 | 0.054 | 0.544 | 18 | 110 | 1.917 |
| rb2/rect | 109 | 0.043 | 0.236 | 9 | 110 | 0.544 |
| rb4/mask | 109 | 0.055 | 0.380 | 17 | 110 | 0.460 |
| rb4/profile | 109 | 0.049 | 0.450 | 17 | 110 | 1.916 |
| rb4/rect | 109 | 0.033 | 0.243 | 10 | 110 | 0.507 |
