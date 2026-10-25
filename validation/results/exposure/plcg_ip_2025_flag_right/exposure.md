# Exposure series: HEK IP FLAG/tubulin, lanes 8-10 (5 s, 20 s)

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `flag_5s.tif` | 5.00000 | - |
| 2 | `flag_20s.tif` | 20.0000 | - |

## Band `r262`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 77.8 | 0.1 | ok |
| 2 (ref) | 0 | 0 | 0 | 135.1 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 3 | 0.081 | 0.142 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | profile | 3 | 0.027 | 0.047 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | mask | 3 | 0.146 | 0.258 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| rb2 | rect | 3 | 0.042 | 0.077 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | profile | 3 | 0.027 | 0.046 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | mask | 3 | 0.545 | 0.613 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| rb4 | rect | 3 | 0.029 | 0.049 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | profile | 3 | 0.032 | 0.049 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | mask | 3 | 0.391 | 0.442 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| local | rect | 3 | 0.033 | 0.055 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | profile | 3 | 0.026 | 0.046 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | mask | 3 | 0.307 | 0.493 | 0 | nan | 0 | 0 | 3 | 0 | 0 |

## Band `r286`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 94.3 | 0.0 | ok |
| 2 (ref) | 0 | 0 | 0 | 166.8 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 3 | 0.030 | 0.032 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | profile | 3 | 0.032 | 0.054 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | mask | 3 | 0.025 | 0.034 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | rect | 3 | 0.044 | 0.044 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | profile | 3 | 0.031 | 0.054 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | mask | 3 | 0.018 | 0.026 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | rect | 3 | 0.059 | 0.104 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | profile | 3 | 0.035 | 0.054 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | mask | 3 | 0.025 | 0.039 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | rect | 3 | 0.011 | 0.021 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | profile | 3 | 0.029 | 0.052 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | mask | 3 | 0.021 | 0.034 | 0 | nan | 0 | 0 | 0 | 3 | 0 |

## Band `tubulin`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|
| 1 (ref) | 0 | 0 | 0 | -52.5 | 0.0 | ok |
| 2 | 0 | 0 | 75 | -111.2 | 0.0 | flagged |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 1 | 0.038 | 0.038 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb1 | profile | 1 | 0.049 | 0.049 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb1 | mask | 1 | 0.031 | 0.031 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | rect | 1 | 0.035 | 0.035 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | profile | 1 | 0.049 | 0.049 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb2 | mask | 1 | 0.031 | 0.031 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb4 | rect | 1 | 0.040 | 0.040 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb4 | profile | 1 | 0.049 | 0.049 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| rb4 | mask | 1 | 0.029 | 0.029 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| local | rect | 1 | 0.037 | 0.037 | 0 | nan | 0 | 0 | 0 | 1 | 2 |
| local | profile | 1 | 0.049 | 0.049 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
| local | mask | 1 | 0.033 | 0.033 | 0 | nan | 0 | 0 | 0 | 1 | 0 |
