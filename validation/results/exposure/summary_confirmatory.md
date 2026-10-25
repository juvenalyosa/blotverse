# Exposure-invariance bench: pooled results

5 series (ufm1_2026_ufm1_a, ufm1_2026_ufm1_c_left, ufm1_2026_ufm1_c_right, ufm1_2026_ufm1_d, ufm1_2026_ufm1_f); default branch `rb2/profile`; distortion threshold 0.2 log2.

## P1. QC misses (v1.2 lane-level rule)

40 usable exposure x band combinations; 5 carry a saturation flag, 5 a saturation or near-saturation flag.

* **Spec rule (judged):** 0 misses
* Candidate near-saturation rule: 0 misses
* v1.1 pair rule, for comparison: 1 misses
  * ufm1_2026_ufm1_f band r246 exposure 2 (WB_UFM1_replicate_3_1.tif)

Not evaluable (a lane saturated at every exposure, so no reference): 0 bands.

S4. Exposures blamed for low-signal distortion: 0

## P2. Compressive-distorted pairs with a flagged lane (default branch)

147 / 148 = 0.993 (Wilson 95 % 0.963-0.999).

## S1-S3. Per branch

| Branch | Clean pairs | median abs delta | q95 abs delta | Clean pairs > 0.2 | Flagged pairs | median abs delta (flagged) |
|---|---|---|---|---|---|---|
| local/mask | 199 | 0.041 | 0.180 | 8 | 228 | 0.205 |
| local/profile | 199 | 0.031 | 0.124 | 1 | 228 | 0.368 |
| local/rect | 199 | 0.043 | 0.107 | 0 | 228 | 0.198 |
| rb1/mask | 199 | 0.037 | 0.153 | 0 | 228 | 0.188 |
| rb1/profile | 199 | 0.032 | 0.125 | 2 | 228 | 0.368 |
| rb1/rect | 199 | 0.025 | 0.080 | 0 | 228 | 0.184 |
| rb2/mask | 199 | 0.032 | 0.160 | 8 | 228 | 0.184 |
| rb2/profile | 199 | 0.032 | 0.125 | 1 | 228 | 0.368 |
| rb2/rect | 199 | 0.015 | 0.067 | 0 | 228 | 0.180 |
| rb4/mask | 199 | 0.030 | 0.181 | 5 | 228 | 0.183 |
| rb4/profile | 199 | 0.031 | 0.124 | 1 | 228 | 0.368 |
| rb4/rect | 199 | 0.017 | 0.060 | 0 | 228 | 0.177 |
