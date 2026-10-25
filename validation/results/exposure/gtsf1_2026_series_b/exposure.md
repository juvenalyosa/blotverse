# Exposure series: GTSF-1 series B (4.5-150 s)

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `anti_GTSF1_5secauto.scn` | 4.45700 | - |
| 2 | `anti_GTSF1_40sec.scn` | 38.2500 | - |
| 3 | `anti_GTSF1_75sec.scn` | 75.5000 | - |
| 4 | `anti_GTSF1_110sec.scn` | 112.750 | - |
| 5 | `anti_GTSF1_150sec.scn` | 150.000 | - |

## Band `upper`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 1283.8 | 0.0 | no flag |
| 2 | 150 | 407 | 145 | 250 | 318 | 16 | 1459.3 | 0.4 | flagged, sub-linear lane |
| 3 | 357 | 585 | 377 | 455 | 530 | 266 | 652.0 | 0.5 | flagged, sub-linear lane |
| 4 | 477 | 642 | 494 | 580 | 599 | 395 | 327.3 | 0.5 | flagged, sub-linear lane |
| 5 | 561 | 693 | 574 | 615 | 643 | 487 | 156.6 | 0.5 | flagged, sub-linear lane |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 0 | nan | nan | 60 | 0.765 | 56 | 4 | 0 | 0 | 0 |
| rb1 | profile | 0 | nan | nan | 60 | 1.766 | 56 | 4 | 0 | 0 | 0 |
| rb1 | mask | 0 | nan | nan | 60 | 0.704 | 54 | 6 | 0 | 0 | 0 |
| rb2 | rect | 0 | nan | nan | 60 | 0.658 | 56 | 4 | 0 | 0 | 0 |
| rb2 | profile | 0 | nan | nan | 60 | 1.766 | 56 | 4 | 0 | 0 | 0 |
| rb2 | mask | 0 | nan | nan | 60 | 0.629 | 53 | 7 | 0 | 0 | 0 |
| rb4 | rect | 0 | nan | nan | 60 | 0.654 | 56 | 4 | 0 | 0 | 0 |
| rb4 | profile | 0 | nan | nan | 60 | 1.766 | 56 | 4 | 0 | 0 | 0 |
| rb4 | mask | 0 | nan | nan | 60 | 0.601 | 53 | 7 | 0 | 0 | 0 |
| local | rect | 0 | nan | nan | 60 | 1.072 | 57 | 3 | 0 | 0 | 0 |
| local | profile | 0 | nan | nan | 60 | 1.766 | 56 | 4 | 0 | 0 | 0 |
| local | mask | 0 | nan | nan | 60 | 1.017 | 55 | 5 | 0 | 0 | 0 |

## Band `lower`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | -15.3 | 0.1 | no flag, low-signal distortion |
| 2 | 0 | 0 | 0 | 0 | 0 | 0 | -19.8 | 0.0 | no flag |
| 3 | 0 | 0 | 0 | 0 | 0 | 0 | -26.0 | 0.0 | no flag |
| 4 | 0 | 0 | 0 | 0 | 0 | 0 | -30.0 | 0.0 | no flag |
| 5 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | -32.3 | 0.0 | no flag |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 24 | 0.014 | 0.401 | 0 | nan | 0 | 0 | 3 | 21 | 0 |
| rb1 | profile | 24 | 0.045 | 0.536 | 0 | nan | 0 | 0 | 5 | 19 | 0 |
| rb1 | mask | 24 | 0.028 | 0.341 | 0 | nan | 0 | 0 | 3 | 21 | 0 |
| rb2 | rect | 24 | 0.035 | 0.449 | 0 | nan | 0 | 0 | 3 | 21 | 0 |
| rb2 | profile | 24 | 0.033 | 1.065 | 0 | nan | 0 | 0 | 5 | 19 | 0 |
| rb2 | mask | 24 | 0.043 | 0.363 | 0 | nan | 0 | 0 | 5 | 19 | 0 |
| rb4 | rect | 24 | 0.021 | 0.069 | 0 | nan | 0 | 0 | 0 | 24 | 0 |
| rb4 | profile | 24 | 0.012 | 0.453 | 0 | nan | 0 | 0 | 3 | 21 | 0 |
| rb4 | mask | 24 | 0.021 | 0.258 | 0 | nan | 0 | 0 | 3 | 21 | 0 |
| local | rect | 24 | 0.027 | 0.406 | 0 | nan | 0 | 0 | 3 | 21 | 20 |
| local | profile | 24 | 0.020 | 0.518 | 0 | nan | 0 | 0 | 3 | 21 | 0 |
| local | mask | 24 | 0.038 | 0.388 | 0 | nan | 0 | 0 | 3 | 21 | 0 |
