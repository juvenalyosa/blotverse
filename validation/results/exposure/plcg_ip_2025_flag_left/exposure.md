# Exposure series: HEK IP FLAG/tubulin, lanes 1-6 (5 s, 20 s)

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `flag_5s.tif` | 5.00000 | - |
| 2 | `flag_20s.tif` | 20.0000 | - |

## Band `r248`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 41 | 0 | 0 | -543.4 | 0.0 | flagged |
| 2 | 263 | 91 | 0 | 910 | 155 | 7 | -438.3 | 0.0 | flagged |

Not evaluable: every exposure has a saturated lane, so no exposure can serve as reference.

## Band `r278`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 386 | 0 | 43 | 623 | 666 | 224.3 | 0.0 | flagged |
| 2 | 0 | 716 | 768 | 770 | 770 | 770 | 0.0 | 0.0 | flagged |

Not evaluable: every exposure has a saturated lane, so no exposure can serve as reference.

## Band `r370`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | -142.1 | 0.1 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | -236.2 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 1 | 0.089 | 0.089 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb1 | profile | 1 | 0.075 | 0.075 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb1 | mask | 1 | 0.089 | 0.089 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | rect | 1 | 0.090 | 0.090 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | profile | 1 | 0.075 | 0.075 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | mask | 1 | 0.751 | 0.751 | 0 | nan | 0 | 0 | 1 | 0 | 0 |
| rb4 | rect | 1 | 0.210 | 0.210 | 0 | nan | 0 | 0 | 1 | 0 | 0 |
| rb4 | profile | 1 | 0.010 | 0.010 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb4 | mask | 1 | 0.678 | 0.678 | 0 | nan | 0 | 0 | 1 | 0 | 0 |
| local | rect | 1 | 0.006 | 0.006 | 0 | nan | 0 | 0 | 0 | 1 | 9 |
| local | profile | 1 | 0.081 | 0.081 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| local | mask | 1 | 0.664 | 0.664 | 0 | nan | 0 | 0 | 1 | 0 | 0 |

## Band `r450`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | -144.2 | 0.0 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | -215.7 | 0.0 | ok |

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
| local | rect | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 15 |
| local | profile | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |
| local | mask | 0 | nan | nan | 0 | nan | 0 | 0 | 0 | 0 | 0 |

## Band `tubulin`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | -31.5 | 0.1 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | -52.2 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 10 | 0.023 | 0.062 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb1 | profile | 10 | 0.044 | 0.084 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb1 | mask | 10 | 0.086 | 0.115 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb2 | rect | 10 | 0.036 | 0.071 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb2 | profile | 10 | 0.042 | 0.083 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb2 | mask | 10 | 0.042 | 0.076 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb4 | rect | 10 | 0.023 | 0.083 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb4 | profile | 10 | 0.042 | 0.083 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb4 | mask | 10 | 0.077 | 0.152 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| local | rect | 10 | 0.033 | 0.062 | 0 | nan | 0 | 0 | 0 | 10 | 5 |
| local | profile | 10 | 0.042 | 0.082 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| local | mask | 10 | 0.055 | 0.117 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
