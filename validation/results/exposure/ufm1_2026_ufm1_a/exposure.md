# Exposure series: UFM1 blot (WB_UFM1), lower and normal exposure

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `WB_UFM1_lower_exposure.tif` | ? | - |
| 2 | `WB_UFM1.tif` | ? | - |

## Band `r223`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | lane 10 | lane 11 | lane 12 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.2 | 0.0 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2.2 | 0.0 | no flag |

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

## Band `r260`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | lane 10 | lane 11 | lane 12 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2.2 | 0.2 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 61.1 | 0.0 | no flag |

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

## Band `doublet`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | lane 10 | lane 11 | lane 12 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 3.6 | 0.0 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 98.9 | 0.0 | no flag |

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
| 1 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 84.9 | 0.0 | no flag |
| 2 | 102 | 123 | 125 | 127 | 115 | 130 | 104 | 140 | 147 | 122 | 119 | 139 | 627.2 | 0.4 | flagged, sub-linear lane |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 0 | nan | nan | 66 | 0.282 | 40 | 26 | 0 | 0 | 0 |
| rb1 | profile | 0 | nan | nan | 66 | 0.329 | 48 | 18 | 0 | 0 | 0 |
| rb1 | mask | 0 | nan | nan | 66 | 0.301 | 46 | 20 | 0 | 0 | 0 |
| rb2 | rect | 0 | nan | nan | 66 | 0.273 | 38 | 28 | 0 | 0 | 0 |
| rb2 | profile | 0 | nan | nan | 66 | 0.329 | 48 | 18 | 0 | 0 | 0 |
| rb2 | mask | 0 | nan | nan | 66 | 0.307 | 45 | 21 | 0 | 0 | 0 |
| rb4 | rect | 0 | nan | nan | 66 | 0.265 | 38 | 28 | 0 | 0 | 0 |
| rb4 | profile | 0 | nan | nan | 66 | 0.329 | 48 | 18 | 0 | 0 | 0 |
| rb4 | mask | 0 | nan | nan | 66 | 0.306 | 45 | 21 | 0 | 0 | 0 |
| local | rect | 0 | nan | nan | 66 | 0.277 | 38 | 28 | 0 | 0 | 0 |
| local | profile | 0 | nan | nan | 66 | 0.329 | 48 | 18 | 0 | 0 | 0 |
| local | mask | 0 | nan | nan | 66 | 0.297 | 44 | 22 | 0 | 0 | 0 |
