# Exposure series: UFM1 replicate 2 (687x550), lanes 1-6

Same membrane at several exposures. Within the linear range the log2 ratio between two lanes does not depend on the exposure; each lane pair at each exposure is compared with the reference exposure (the longest one with no pixel at the detector maximum in any band window). Pairs with a lane below SNR 10 are left out; a pair is *distorted* when its ratio moves by more than 0.2 log2 and *flagged* when the saturation QC rule (>= 1 px at the detector maximum) excludes either lane.

## Exposures

| # | Image | Seconds | Note |
|---|---|---|---|
| 1 | `WB_UFM1_lower_exposure_replicate_2.tif` | ? | - |
| 2 | `WB_UFM1_replicate_2.tif` | ? | - |

## Band `upper`

Saturated pixels per lane window, by exposure:

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 1.7 | 0.0 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 47.9 | 0.0 | no flag |

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

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 | 0 | 0 | 0 | 0 | 0 | 0 | 5.4 | 0.0 | no flag |
| 2 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 148.7 | 0.0 | no flag |

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

| # | lane 1 | lane 2 | lane 3 | lane 4 | lane 5 | lane 6 | min SNR | max drift (px) | G3 (rb2/profile) |
|---|---|---|---|---|---|---|---|---|---|
| 1 (ref) | 0 | 0 | 0 | 0 | 0 | 0 | 215.5 | 0.0 | no flag |
| 2 | 185 | 194 | 217 | 203 | 212 | 177 | 601.8 | 0.5 | flagged |

| Background | Band definition | Clean pairs | median abs delta | q95 abs delta | Flagged pairs | median abs delta (flagged) | QC TP | FP | FN | TN | Non-positive |
|---|---|---|---|---|---|---|---|---|---|---|---|
| rb1 | rect | 0 | nan | nan | 15 | 0.153 | 4 | 11 | 0 | 0 | 0 |
| rb1 | profile | 0 | nan | nan | 15 | 0.209 | 9 | 6 | 0 | 0 | 0 |
| rb1 | mask | 0 | nan | nan | 15 | 0.204 | 8 | 7 | 0 | 0 | 0 |
| rb2 | rect | 0 | nan | nan | 15 | 0.139 | 4 | 11 | 0 | 0 | 0 |
| rb2 | profile | 0 | nan | nan | 15 | 0.208 | 9 | 6 | 0 | 0 | 0 |
| rb2 | mask | 0 | nan | nan | 15 | 0.183 | 6 | 9 | 0 | 0 | 0 |
| rb4 | rect | 0 | nan | nan | 15 | 0.141 | 4 | 11 | 0 | 0 | 0 |
| rb4 | profile | 0 | nan | nan | 15 | 0.208 | 9 | 6 | 0 | 0 | 0 |
| rb4 | mask | 0 | nan | nan | 15 | 0.170 | 6 | 9 | 0 | 0 | 0 |
| local | rect | 0 | nan | nan | 15 | 0.187 | 6 | 9 | 0 | 0 | 0 |
| local | profile | 0 | nan | nan | 15 | 0.208 | 9 | 6 | 0 | 0 | 0 |
| local | mask | 0 | nan | nan | 15 | 0.214 | 8 | 7 | 0 | 0 | 0 |
