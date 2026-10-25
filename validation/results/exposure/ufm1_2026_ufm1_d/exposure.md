# Exposure series: UFM1 replicate 2 (916x733)

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `WB_UFM1_lower_exposure_replicate_2_1.tif` | ? | - |
| 2 | `WB_UFM1_replicate_2_1.tif` | ? | - |

## Band `r154`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | lane 10 | lane 11 | lane 12 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.2 | 0.0 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.8 | 0.0 | no flag |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 1 | 0.001 | 0.001 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb1 | profile | 1 | 0.041 | 0.041 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb1 | mask | 1 | 0.103 | 0.103 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | rect | 1 | 0.028 | 0.028 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | profile | 1 | 0.043 | 0.043 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | mask | 1 | 0.092 | 0.092 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb4 | rect | 1 | 0.074 | 0.074 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb4 | profile | 1 | 0.030 | 0.030 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb4 | mask | 1 | 0.160 | 0.160 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| local | rect | 1 | 0.054 | 0.054 | 0 | nan | 0 | 0 | 0 | 1 | 11 |
| local | profile | 1 | 0.041 | 0.041 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| local | mask | 1 | 0.065 | 0.065 | 0 | nan | 0 | 0 | 0 | 1 | 0 |

## Band `r182`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | lane 10 | lane 11 | lane 12 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -0.6 | 0.0 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -3.2 | 0.0 | no flag |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 15 | 0.045 | 0.092 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb1 | profile | 15 | 0.067 | 0.135 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb1 | mask | 15 | 0.026 | 0.064 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb2 | rect | 15 | 0.015 | 0.039 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb2 | profile | 15 | 0.067 | 0.134 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb2 | mask | 15 | 0.024 | 0.078 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb4 | rect | 15 | 0.017 | 0.043 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb4 | profile | 15 | 0.067 | 0.134 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb4 | mask | 15 | 0.024 | 0.061 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| local | rect | 15 | 0.056 | 0.107 | 0 | nan | 0 | 0 | 0 | 15 | 21 |
| local | profile | 15 | 0.067 | 0.133 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| local | mask | 15 | 0.033 | 0.081 | 0 | nan | 0 | 0 | 0 | 15 | 21 |

## Band `doublet`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | lane 10 | lane 11 | lane 12 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 19.6 | 0.1 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 66.2 | 0.0 | no flag |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 66 | 0.017 | 0.051 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| rb1 | profile | 66 | 0.032 | 0.105 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| rb1 | mask | 66 | 0.038 | 0.173 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| rb2 | rect | 66 | 0.010 | 0.026 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| rb2 | profile | 66 | 0.032 | 0.105 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| rb2 | mask | 66 | 0.037 | 0.218 | 0 | nan | 0 | 0 | 8 | 58 | 0 |
| rb4 | rect | 66 | 0.014 | 0.033 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| rb4 | profile | 66 | 0.031 | 0.104 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| rb4 | mask | 66 | 0.032 | 0.214 | 0 | nan | 0 | 0 | 5 | 61 | 0 |
| local | rect | 66 | 0.044 | 0.112 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| local | profile | 66 | 0.031 | 0.104 | 0 | nan | 0 | 0 | 0 | 66 | 0 |
| local | mask | 66 | 0.053 | 0.249 | 0 | nan | 0 | 0 | 8 | 58 | 0 |

## Band `r251`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | lane 10 | lane 11 | lane 12 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -0.1 | 0.0 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -0.0 | 0.0 | no flag |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| rb1 | profile | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| rb1 | mask | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| rb2 | rect | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| rb2 | profile | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| rb2 | mask | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| rb4 | rect | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| rb4 | profile | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| rb4 | mask | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| local | rect | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| local | profile | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| local | mask | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |

## Band `low`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | lane 10 | lane 11 | lane 12 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 202.7 | 0.0 | no flag |
| 2 | 79 | 162 | 175 | 175 | 186 | 178 | 147 | 184 | 169 | 155 | 201 | 196 | 555.1 | 0.2 | flagged, sub-linear lane |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 0 | nan | nan | 66 | 0.186 | 28 | 38 | 0 | 0 | 0 |
| rb1 | profile | 0 | nan | nan | 66 | 0.352 | 53 | 13 | 0 | 0 | 0 |
| rb1 | mask | 0 | nan | nan | 66 | 0.138 | 21 | 45 | 0 | 0 | 0 |
| rb2 | rect | 0 | nan | nan | 66 | 0.184 | 27 | 39 | 0 | 0 | 0 |
| rb2 | profile | 0 | nan | nan | 66 | 0.352 | 53 | 13 | 0 | 0 | 0 |
| rb2 | mask | 0 | nan | nan | 66 | 0.125 | 17 | 49 | 0 | 0 | 0 |
| rb4 | rect | 0 | nan | nan | 66 | 0.182 | 27 | 39 | 0 | 0 | 0 |
| rb4 | profile | 0 | nan | nan | 66 | 0.352 | 53 | 13 | 0 | 0 | 0 |
| rb4 | mask | 0 | nan | nan | 66 | 0.123 | 17 | 49 | 0 | 0 | 0 |
| local | rect | 0 | nan | nan | 66 | 0.204 | 34 | 32 | 0 | 0 | 0 |
| local | profile | 0 | nan | nan | 66 | 0.352 | 53 | 13 | 0 | 0 | 0 |
| local | mask | 0 | nan | nan | 66 | 0.145 | 27 | 39 | 0 | 0 | 0 |
