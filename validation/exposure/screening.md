# Exposure bench: screening of the confirmatory candidates

The candidates are those listed in `docs/exposure_protocol.md`, screened in that order. The
"Criterion" column refers to the protocol's inclusion criteria.

* **Format** was checked with BlotVerse's reader.
* **Same membrane and alignment** were checked by phase correlation and visual inspection.

| # | Study (EMBO SourceData) | Files | Result | Criterion / reason |
|---|---|---|---|---|
| 1 | S44321-025-00317-0 | pSTAT1/5/6 short/long exposure TIFFs (several pairs) | excluded | 4: every file is 8-bit (256 grey levels) |
| 2 | S44321-026-00493-7 | Western_FOSL1 long/short exposure | excluded | 4: 8-bit |
| 3 | S44319-026-00904-3 | BRCA1 long/short exposure (Fig 2A, 2D) | excluded | 4: the long exposures are 8-bit; 2: the pairs have different dimensions |
| 4 | S44319-026-00783-8 | STREMI / StrepII high/low exposure | excluded | 4: STREMI-High is 8-bit RGB; 5: the 16-bit pairs show a continuous strip with no separable lanes |
| 5 | S44318-025-00602-1 | 0.2 s / 0.5 s / 45 s | excluded | 5: lanes run horizontally (would need rotation support) |
| 6 | S44319-026-00901-6 | GFPSet1IP-GFPBlot-1 (20 s) and -2 (294.7 s), raw16 | **included** | one 916x733 membrane, aligned (shift 0, 0); `set1ip_2026/gfp.yaml` |
| 7 | S44318-025-00677-W | FBP17 "titration" | excluded | not a western blot (microscopy) |
| 8 | S44318-025-00566-2 | 0 s ... 264 s | excluded | not a western blot (time-lapse microscopy) |
| 9 | S44318-025-00643-6 | 2s ... 6s | excluded | 4: pseudo-coloured, rejected by the reader |
| 10 | S44318-025-00585-Z | 10 s, 2 s, 150 s | excluded | 1: one exposure per blot (different blots and dates) |
| 11 | S44319-026-00745-0 | 4B_stats 1.0 s / 60.0 s | excluded | 4: 8-bit RGB composites |
| 12 | S44318-026-00855-4 | 15 nm / 40 nm, 0.1–339 s | excluded | not a western blot (about 90x90 px images in an unsupported sample format) |
| 13 | S44319-025-00550-1 | Chemiluminescence 18.8 s / 200 s (Composite) | excluded | 4: 8-bit RGB composites |
| 14 | S44318-025-00600-3 | hek_ip_plctub and hek_ip_wt_flagtub, 5 s / 20 s, "_Ch" | **included** | two 1408x1024 membranes, aligned; `plcg_ip_2025/plc.yaml`, `flag_left.yaml`, `flag_right.yaml` (the FLAG membrane is tilted, so lanes 1–6 and 8–10 are separate configs) |
| 15 | S44318-025-00507-Z | IL1b_Goat_HRP 15 s / 600 s | excluded | 4: pseudo-coloured, rejected by the reader |
| 16 | S44321-025-00357-6 | DMSO / UNC0642 low/higher exposure | excluded | 4: 8-bit RGB |

**Yield:** 2 of 16 studies, giving 4 series and 3 membranes. The usual reason for exclusion
is that only 8-bit or colour exports were deposited, not the 16-bit acquisition. This
matches what the level-3 search found.
