# Exposure series: GFP-Set1 IP anti-GFP (20 s, 295 s)

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `gfp_20s.tif` | 20.0000 | - |
| 2 | `gfp_295s.tif` | 294.734 | - |

## Band `r188`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -13.3 | 0.1 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -17.9 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 3 | 0.195 | 0.273 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| rb1 | profile | 3 | 0.093 | 0.136 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | mask | 3 | 0.235 | 0.245 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| rb2 | rect | 3 | 0.242 | 0.294 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| rb2 | profile | 3 | 0.209 | 0.231 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| rb2 | mask | 3 | 0.290 | 0.447 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| rb4 | rect | 3 | 0.216 | 0.265 | 0 | nan | 0 | 0 | 2 | 1 | 0 |
| rb4 | profile | 3 | 0.139 | 0.151 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | mask | 3 | 0.275 | 0.487 | 0 | nan | 0 | 0 | 3 | 0 | 0 |
| local | rect | 3 | 0.196 | 0.243 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| local | profile | 3 | 0.097 | 0.109 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | mask | 3 | 0.218 | 0.293 | 0 | nan | 0 | 0 | 2 | 1 | 0 |

## Band `r212`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -0.4 | 0.0 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -0.5 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 3 | 0.049 | 0.077 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | profile | 3 | 0.047 | 0.062 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb1 | mask | 3 | 0.066 | 0.108 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | rect | 3 | 0.133 | 0.154 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | profile | 3 | 0.056 | 0.084 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb2 | mask | 3 | 0.157 | 0.183 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | rect | 3 | 0.105 | 0.200 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| rb4 | profile | 3 | 0.032 | 0.053 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| rb4 | mask | 3 | 0.156 | 0.205 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| local | rect | 3 | 0.173 | 0.315 | 0 | nan | 0 | 0 | 1 | 2 | 0 |
| local | profile | 3 | 0.037 | 0.054 | 0 | nan | 0 | 0 | 0 | 3 | 0 |
| local | mask | 3 | 0.154 | 0.196 | 0 | nan | 0 | 0 | 1 | 2 | 0 |

## Band `r275`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -2.3 | 0.0 | ok |
| 2 | 0 | 0 | 98 | 0 | 0 | 0 | 0 | 0 | 0 | -4.3 | 0.0 | flagged |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 0 | nan | nan | 1 | 0.073 | 0 | 1 | 0 | 0 | 0 |
| rb1 | profile | 0 | nan | nan | 1 | 0.271 | 1 | 0 | 0 | 0 | 0 |
| rb1 | mask | 0 | nan | nan | 1 | 0.168 | 0 | 1 | 0 | 0 | 0 |
| rb2 | rect | 0 | nan | nan | 1 | 0.085 | 0 | 1 | 0 | 0 | 0 |
| rb2 | profile | 0 | nan | nan | 1 | 0.243 | 1 | 0 | 0 | 0 | 0 |
| rb2 | mask | 0 | nan | nan | 1 | 0.156 | 0 | 1 | 0 | 0 | 0 |
| rb4 | rect | 0 | nan | nan | 1 | 0.061 | 0 | 1 | 0 | 0 | 0 |
| rb4 | profile | 0 | nan | nan | 1 | 0.244 | 1 | 0 | 0 | 0 | 0 |
| rb4 | mask | 0 | nan | nan | 1 | 0.182 | 0 | 1 | 0 | 0 | 0 |
| local | rect | 0 | nan | nan | 1 | 0.006 | 0 | 1 | 0 | 0 | 8 |
| local | profile | 0 | nan | nan | 1 | 0.244 | 1 | 0 | 0 | 0 | 0 |
| local | mask | 0 | nan | nan | 1 | 0.141 | 0 | 1 | 0 | 0 | 0 |

## Band `r292`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 1.6 | 0.0 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 2.1 | 0.0 | ok |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 6 | 0.115 | 0.207 | 0 | nan | 0 | 0 | 1 | 5 | 0 |
| rb1 | profile | 6 | 0.110 | 0.139 | 0 | nan | 0 | 0 | 0 | 6 | 0 |
| rb1 | mask | 6 | 0.043 | 0.058 | 0 | nan | 0 | 0 | 0 | 6 | 0 |
| rb2 | rect | 6 | 0.110 | 0.163 | 0 | nan | 0 | 0 | 0 | 6 | 0 |
| rb2 | profile | 6 | 0.077 | 0.141 | 0 | nan | 0 | 0 | 0 | 6 | 0 |
| rb2 | mask | 6 | 0.451 | 0.860 | 0 | nan | 0 | 0 | 3 | 3 | 0 |
| rb4 | rect | 6 | 0.123 | 0.226 | 0 | nan | 0 | 0 | 1 | 5 | 0 |
| rb4 | profile | 6 | 0.099 | 0.193 | 0 | nan | 0 | 0 | 0 | 6 | 0 |
| rb4 | mask | 6 | 0.276 | 0.513 | 0 | nan | 0 | 0 | 4 | 2 | 0 |
| local | rect | 6 | 0.131 | 0.248 | 0 | nan | 0 | 0 | 2 | 4 | 0 |
| local | profile | 6 | 0.057 | 0.112 | 0 | nan | 0 | 0 | 0 | 6 | 0 |
| local | mask | 6 | 0.168 | 0.319 | 0 | nan | 0 | 0 | 3 | 3 | 0 |

## Band `doublet`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0.1 | 0.2 | ok |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | 0 | -0.9 | 0.0 | **MISS: distorted, not flagged** |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 15 | 0.102 | 0.217 | 0 | nan | 0 | 0 | 1 | 14 | 0 |
| rb1 | profile | 15 | 0.095 | 0.565 | 0 | nan | 0 | 0 | 5 | 10 | 0 |
| rb1 | mask | 15 | 0.127 | 0.247 | 0 | nan | 0 | 0 | 2 | 13 | 0 |
| rb2 | rect | 15 | 0.080 | 0.236 | 0 | nan | 0 | 0 | 3 | 12 | 0 |
| rb2 | profile | 15 | 0.103 | 0.593 | 0 | nan | 0 | 0 | 5 | 10 | 0 |
| rb2 | mask | 15 | 0.141 | 0.310 | 0 | nan | 0 | 0 | 5 | 10 | 0 |
| rb4 | rect | 15 | 0.096 | 0.350 | 0 | nan | 0 | 0 | 5 | 10 | 0 |
| rb4 | profile | 15 | 0.263 | 0.732 | 0 | nan | 0 | 0 | 9 | 6 | 0 |
| rb4 | mask | 15 | 0.171 | 0.341 | 0 | nan | 0 | 0 | 3 | 12 | 0 |
| local | rect | 15 | 0.090 | 0.536 | 0 | nan | 0 | 0 | 5 | 10 | 21 |
| local | profile | 15 | 0.064 | 0.454 | 0 | nan | 0 | 0 | 5 | 10 | 0 |
| local | mask | 15 | 0.161 | 0.362 | 0 | nan | 0 | 0 | 6 | 9 | 0 |

## Band `gfp`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | lane 7 | lane 8 | lane 9 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|---|---|---|
| 1 | 78 | 0 | 0 | 150 | 0 | 0 | 0 | 0 | 0 | -2.0 | 0.0 | flagged |
| 2 | 352 | 0 | 28 | 352 | 12 | 0 | 338 | 1 | 0 | -3.2 | 0.0 | flagged |

Not evaluable: every exposure has a saturated lane, so no exposure can serve as reference.
