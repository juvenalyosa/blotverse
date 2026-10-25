# Exposure-invariance bench: protocol (pre-specified)

Status: **fixed before the confirmatory series were screened.** This file, the
`blotverse exposure` command and `validation/exposure/summarize.py` were committed before
any confirmatory series was measured. Two of them had already been downloaded and checked
for format only:

* S44319-026-00783-8: `imagestats`.
* S44318-025-00602-1: `imagestats` and one preview, which showed horizontal lanes.

No band in them had been measured. The development series (GTSF-1 A and
B) shaped two definitions, the band-prominence SNR and the blame rule, so they are
**exploratory** and are reported separately.

## Why exposures give a known truth

The same membrane imaged at several exposures holds the same protein in every lane. Inside
the detector's linear range, the ratio between two lanes of one band does not depend on the
exposure. Each exposure is therefore a replicate of the others with a known answer: **no
change**. Departures from it come from three sources:

* **Saturation or detector non-linearity.** The brighter lane loses ground and the ratio
  moves towards 1.
* **Noise.** It matters at short exposures and for faint bands.
* **Background handling.** The background grows with exposure (membrane glow) and
  subtracting it imperfectly does not scale.

The first is what the plan's G3 clause "all saturated exposures flagged" is about. The
other two measure how well each background x band-definition branch preserves ratios.

No laboratory work is needed: the series come from published, raw, uncropped acquisitions.

## Inclusion criteria (per series)

1. **One membrane** imaged at ≥ 2 exposures, from the authors' deposited raw files.
2. **Same pixel grid:** identical dimensions. The command excludes other images, for
   example a different binning, and records why.
3. **Aligned:** the band centroid drifts ≤ 2 px from the reference exposure. The command
   excludes misaligned exposures.
4. **Readable by BlotVerse without rejection:** 16-bit or float data, not 8-bit and not
   upscaled 8-bit. An inverted image is accepted with `invert: true`.
5. **Vertical lanes** that can be told apart, and at least one discrete band row present in
   ≥ 2 lanes.

## Annotation rules

Annotation is done by one annotator, who is not blinded to saturation: it is visible in the
image.

* **Lanes:** every sample lane; molecular-weight ladders are excluded. Centres come from
  column profiles of the shortest exposure; the lane width is about 80–90 % of the lane
  pitch.
* **Bands:** every discrete band row that is visible in ≥ 2 lanes of the shortest
  exposure. Rows come from row profiles; the half-height is about the band's half width at
  half maximum.
* **Geometry:**
  * set once. `refine_rows` (≤ 8) is used only when bands are tilted, and only on the first
    listed exposure, which must be the shortest;
  * then held fixed for every exposure;
  * not changed after looking at results.

## Fixed parameters

| Parameter | Value |
|---|---|
| Spec | `spec/multiverse.yaml` v1.2.0 (sha256 `50bbba7dacbcb1aa…`), all 4 background x 3 band-definition branches |
| Saturation QC rule | ≥ 1 pixel at the detector maximum in the band window (spec `qc.saturation_min_pixels`) |
| Reference exposure | the longest exposure with no flagged lane in that band |
| SNR | band prominence: mean of the central rows minus the flanking rows, divided by the image's robust noise (1.4826 x MAD) |
| SNR floor | 10 at both exposures of a pair; pairs below it are *dim* and left out of every endpoint |
| Distortion threshold | \|Δ\| > 0.2 log2 (about 15 %), where Δ = log2(S_i/S_j) at an exposure minus at the reference |
| Compressive | distorted, and the ratio is closer to 1 at the longer of the two exposures; blamed on the longer exposure |
| Default branch for G3 | the spec default, `rb2/profile` |
| Maximum drift | 2 px |

## Endpoints

**Primary:**

* **P1. QC misses.** Count the exposure x band combinations (usable, aligned) that have a
  compressive distortion in the default branch and no flagged lane.
  * **Criterion (G3 clause): zero misses** in the confirmatory set.
  * Each miss is listed with its series, band and exposure.
* **P2. Compressive-distorted pairs with a flagged lane** (default branch), with a Wilson
  95 % interval. Descriptive; expected near 1 if P1 holds.

**Secondary (descriptive, per branch, pooled over series):**

* **S1.** \|Δ\| of clean pairs (no flagged lane, above the SNR floor): n, median and 95th
  percentile. It ranks branches by how well they preserve ratios.
* **S2.** Median \|Δ\| of flagged pairs: how much saturation damages each branch.
* **S3.** Clean pairs with \|Δ\| > 0.2: distortion that is not saturation.

## Sets

* **Development (exploratory):** GTSF-1 series A and B (EMBO Rep 2026,
  doi:10.1038/s44319-026-00914-1).
* **Confirmatory:** the exposure series found in EMBO SourceData (scan of 1087 studies,
  `docs/real_data.md`). They are screened in this order:
  1. S44321-025-00317-0 (pSTAT1 short/long)
  2. S44321-026-00493-7 (FOSL1 long/short)
  3. S44319-026-00904-3 (BRCA1 long/short)
  4. S44319-026-00783-8 (STREMI/StrepII high/low)
  5. S44318-025-00602-1 (0.2 s / 0.5 s / 45 s)
  6. S44319-026-00901-6
  7. S44318-025-00677-W (FBP17 titration)
  8. S44318-025-00566-2
  9. S44318-025-00643-6
  10. S44318-025-00585-Z
  11. S44318-025-00745-0
  12. S44318-025-00855-4
  13. S44318-025-00550-1
  14. S44318-025-00600-3
  15. S44318-025-00507-Z
  16. S44321-025-00357-6

  Every screened series is reported, whether included or excluded, with the reason for any
  exclusion.

## Limitations

* **One annotator, not blinded.**
* **The truth is "no change", not a known fold change.** The bench tests invariance and the
  QC, not accuracy at a given effect size; the Pillai-Kastoori dilution series covers that.
* **Cumulative series from one acquisition.** In these, substrate kinetics are shared
  across exposures. Separate captures can differ for chemical reasons (substrate decay
  between captures). That would appear as non-compressive distortion and is reported under
  S3, not P1.
* **Hidden saturation.** If a vendor's processing (for example flat-field correction)
  stores saturated pixels below the detector maximum, the QC rule cannot see them. P1 is
  designed to expose exactly that case; the synthetic test
  `exposure bench: clipping below the detector maximum is reported as a QC miss` shows that
  the bench detects it.
