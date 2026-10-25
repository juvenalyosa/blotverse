# Exposure series: HEK IP PLC/tubulin (5 s, 20 s)

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `plc_5s.tif` | 5.00000 | - |
| 2 | `plc_20s.tif` | 20.0000 | - |

## Band `r264`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 37.9 | 0.0 | ok |
| 2 | 0 | 734 | 716 | 0 | 0 | 0 | 60.6 | 0.1 | flagged, distorted |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 6 | 0.043 | 0.060 | 9 | 0.329 | 9 | 0 | 0 | 6 | 0 |
| rb1 | profile | 6 | 0.032 | 0.061 | 9 | 0.758 | 9 | 0 | 0 | 6 | 0 |
| rb1 | mask | 6 | 0.049 | 0.095 | 9 | 0.055 | 0 | 9 | 0 | 6 | 0 |
| rb2 | rect | 6 | 0.024 | 0.048 | 9 | 0.257 | 8 | 1 | 0 | 6 | 0 |
| rb2 | profile | 6 | 0.032 | 0.061 | 9 | 0.758 | 9 | 0 | 0 | 6 | 0 |
| rb2 | mask | 6 | 0.057 | 0.103 | 9 | 0.104 | 1 | 8 | 0 | 6 | 0 |
| rb4 | rect | 6 | 0.025 | 0.046 | 9 | 0.243 | 7 | 2 | 0 | 6 | 0 |
| rb4 | profile | 6 | 0.032 | 0.060 | 9 | 0.757 | 9 | 0 | 0 | 6 | 0 |
| rb4 | mask | 6 | 0.048 | 0.091 | 9 | 0.158 | 1 | 8 | 0 | 6 | 0 |
| local | rect | 6 | 0.032 | 0.062 | 9 | 0.420 | 9 | 0 | 0 | 6 | 0 |
| local | profile | 6 | 0.032 | 0.060 | 9 | 0.757 | 9 | 0 | 0 | 6 | 0 |
| local | mask | 6 | 0.048 | 0.088 | 9 | 0.200 | 5 | 4 | 0 | 6 | 0 |

## Band `r352`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | -4.9 | 0.0 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | -6.6 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 3 | 0.015 | 0.019 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | profile | 3 | 0.005 | 0.008 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | mask | 3 | 0.040 | 0.054 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | rect | 3 | 0.014 | 0.016 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | profile | 3 | 0.005 | 0.008 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | mask | 3 | 0.014 | 0.022 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | rect | 3 | 0.024 | 0.043 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | profile | 3 | 0.005 | 0.008 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | mask | 3 | 0.030 | 0.033 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | rect | 3 | 0.005 | 0.010 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | profile | 3 | 0.005 | 0.008 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | mask | 3 | 0.016 | 0.022 | 0 | nan | 0 | 0 | 0 | 3 | 0 |

## Band `r404`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 1.6 | 0.0 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 1.9 | 0.0 | **MISS: distorted, not flagged** |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 3 | 0.079 | 0.111 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | profile | 3 | 0.214 | 0.264 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| rb1 | mask | 3 | 0.154 | 0.231 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| rb2 | rect | 3 | 0.058 | 0.076 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | profile | 3 | 0.157 | 0.257 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| rb2 | mask | 3 | 0.078 | 0.136 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | rect | 3 | 0.040 | 0.046 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | profile | 3 | 0.146 | 0.202 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| rb4 | mask | 3 | 0.086 | 0.107 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | rect | 3 | 0.065 | 0.076 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | profile | 3 | 0.215 | 0.263 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| local | mask | 3 | 0.118 | 0.144 | 0 | nan | 0 | 0 | 0 | 3 | 0 |

## Band `r444`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 19.1 | 0.1 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 28.6 | 0.0 | **MISS: distorted, not flagged** |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 15 | 0.122 | 0.256 | 0 | nan | 0 | 0 | 3 | 12 | 0 |
| rb1 | profile | 15 | 0.155 | 0.369 | 0 | nan | 0 | 0 | 7 | 8 | 0 |
| rb1 | mask | 15 | 0.056 | 0.262 | 0 | nan | 0 | 0 | 5 | 10 | 0 |
| rb2 | rect | 15 | 0.063 | 0.184 | 0 | nan | 0 | 0 | 1 | 14 | 0 |
| rb2 | profile | 15 | 0.157 | 0.337 | 0 | nan | 0 | 0 | 5 | 10 | 0 |
| rb2 | mask | 15 | 0.129 | 0.245 | 0 | nan | 0 | 0 | 3 | 12 | 0 |
| rb4 | rect | 15 | 0.020 | 0.071 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| rb4 | profile | 15 | 0.139 | 0.268 | 0 | nan | 0 | 0 | 4 | 11 | 0 |
| rb4 | mask | 15 | 0.069 | 0.145 | 0 | nan | 0 | 0 | 0 | 15 | 0 |
| local | rect | 15 | 0.160 | 0.268 | 0 | nan | 0 | 0 | 4 | 11 | 0 |
| local | profile | 15 | 0.139 | 0.267 | 0 | nan | 0 | 0 | 4 | 11 | 0 |
| local | mask | 15 | 0.074 | 0.152 | 0 | nan | 0 | 0 | 0 | 15 | 0 |

## Band `tubulin`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 2.7 | 0.0 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 2.7 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 10 | 0.016 | 0.065 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb1 | profile | 10 | 0.018 | 0.147 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb1 | mask | 10 | 0.041 | 0.078 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb2 | rect | 10 | 0.013 | 0.018 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb2 | profile | 10 | 0.023 | 0.124 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb2 | mask | 10 | 0.030 | 0.104 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb4 | rect | 10 | 0.004 | 0.010 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb4 | profile | 10 | 0.023 | 0.127 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| rb4 | mask | 10 | 0.021 | 0.057 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| local | rect | 10 | 0.030 | 0.070 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| local | profile | 10 | 0.023 | 0.125 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
| local | mask | 10 | 0.013 | 0.033 | 0 | nan | 0 | 0 | 0 | 10 | 0 |
