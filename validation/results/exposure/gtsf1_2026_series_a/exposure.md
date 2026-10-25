# Exposure series: GTSF-1 series A (1-200 s, cumulative)

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `08.03.anti_gtsf1_1sec.scn` | 1.00000 | - |
| 2 | `08.03.anti_gtsf1_50sec.scn` | 50.7500 | - |
| 3 | `08.03.anti_gtsf1_100sec.scn` | 100.500 | - |
| 4 | `08.03.anti_gtsf1_150sec.scn` | 150.250 | - |
| 5 | `08.03.anti_gtsf1_200sec.scn` | 200.000 | - |
| 6 | `08.03.anti_gtsf1_5sec.scn` | 4.87900 | excluded: 687x550 differs from the first exposure |

## Band `upper`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|
| 1 (ref) | 0 | 0 | 0 | 0 | 0 | 113.3 | 0.0 | ok |
| 2 | 223 | 411 | 325 | 424 | 139 | 2112.3 | 0.5 | flagged, distorted |
| 3 | 450 | 584 | 571 | 610 | 370 | 1041.7 | 0.8 | flagged, distorted |
| 4 | 565 | 638 | 659 | 704 | 492 | 563.3 | 1.1 | flagged, distorted |
| 5 | 638 | 678 | 709 | 718 | 550 | 320.5 | 1.3 | flagged, distorted |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 0 | nan | nan | 40 | 0.598 | 33 | 7 | 0 | 0 | 0 |
| rb1 | profile | 0 | nan | nan | 40 | 2.878 | 39 | 1 | 0 | 0 | 0 |
| rb1 | mask | 0 | nan | nan | 40 | 0.713 | 36 | 4 | 0 | 0 | 0 |
| rb2 | rect | 0 | nan | nan | 40 | 0.547 | 30 | 10 | 0 | 0 | 0 |
| rb2 | profile | 0 | nan | nan | 40 | 2.877 | 39 | 1 | 0 | 0 | 0 |
| rb2 | mask | 0 | nan | nan | 40 | 0.417 | 27 | 13 | 0 | 0 | 0 |
| rb4 | rect | 0 | nan | nan | 40 | 0.515 | 30 | 10 | 0 | 0 | 0 |
| rb4 | profile | 0 | nan | nan | 40 | 2.877 | 39 | 1 | 0 | 0 | 0 |
| rb4 | mask | 0 | nan | nan | 40 | 0.390 | 27 | 13 | 0 | 0 | 0 |
| local | rect | 0 | nan | nan | 40 | 0.834 | 34 | 6 | 0 | 0 | 0 |
| local | profile | 0 | nan | nan | 40 | 2.877 | 39 | 1 | 0 | 0 | 0 |
| local | mask | 0 | nan | nan | 40 | 0.682 | 33 | 7 | 0 | 0 | 0 |

## Band `lower`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | -2.0 | 0.0 | ok |
| 2 | 0 | 0 | 0 | 0 | 0 | -44.8 | 0.0 | ok |
| 3 | 0 | 0 | 0 | 0 | 0 | -59.7 | 0.0 | ok |
| 4 | 0 | 0 | 0 | 0 | 0 | -68.6 | 0.0 | ok |
| 5 (ref) | 0 | 0 | 0 | 0 | 0 | -75.2 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 3 | 0.004 | 0.006 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | profile | 3 | 0.005 | 0.006 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | mask | 3 | 0.004 | 0.015 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | rect | 3 | 0.005 | 0.017 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | profile | 3 | 0.005 | 0.006 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | mask | 3 | 0.041 | 0.042 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | rect | 3 | 0.001 | 0.002 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | profile | 3 | 0.006 | 0.008 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | mask | 3 | 0.018 | 0.022 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | rect | 3 | 0.014 | 0.020 | 0 | nan | 0 | 0 | 0 | 3 | 36 |
| local | profile | 3 | 0.005 | 0.006 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | mask | 3 | 0.039 | 0.042 | 0 | nan | 0 | 0 | 0 | 3 | 28 |
