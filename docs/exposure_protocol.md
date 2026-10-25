# Exposure-invariance bench: protocol (pre-specified, amended)

Status:

* **v1.0** was fixed before its confirmatory series were screened (commit b56a6d2).
* **v1.1** amended it after screening; see [Amendments](#amendments).
* **v1.2** ([below](#v12-2026-10-25-new-primary-rule-and-new-confirmatory-set)) replaces
  the primary rule. It was designed on all six v1.1 series and is fixed before a new
  confirmatory set is screened. Where v1.2 differs from the text above, v1.2 governs. This file, the
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
| SNR | band prominence: mean of the central rows minus the flanking rows, divided by the image's robust pixel noise (v1.1: 1.4826 x MAD of horizontal first differences / √2; v1.0: 1.4826 x MAD of the raw intensities) |
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
  11. S44319-026-00745-0
  12. S44318-026-00855-4
  13. S44319-025-00550-1
  14. S44318-025-00600-3
  15. S44318-025-00507-Z
  16. S44321-025-00357-6

  Every screened series is reported, whether included or excluded, with the reason for any
  exclusion: [`validation/exposure/screening.md`](../validation/exposure/screening.md).
  Three accessions were mistyped in v1.0 (00745-0, 00855-4 and 00550-1 had the wrong journal
  or year prefix). They are corrected above; the studies are the same.

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

## Amendments

### v1.1 (2026-10-25), after screening, before pooling

Three changes were made after confirmatory cases 6 (`set1ip_2026`) and 14 (`plcg_ip_2025`)
had been run and their per-band tables seen. No pooled P1 or P2 had been computed.

1. **Noise for the SNR.** v1.0 used the whole-image MAD of raw intensities.
   * When the membrane fills the frame (case 14), that MAD measures membrane structure, not
     pixel noise: clearly visible tubulin bands scored a prominence of 3–5.
   * v1.1 uses the MAD of horizontal first differences. That is 1.9–7.8x smaller on the
     development images and about 60x smaller on case 14.
   * The SNR floor stays at 10. It now admits more pairs, so it is not the same floor in
     units of pixel noise.
   * First differences underestimate the noise when neighbouring pixels are correlated
     (smoothed exports); the bench's images are unsmoothed camera data.
2. **Alignment check.** v1.0 compared band centroids in every lane, including lanes without
   the band, where the "centroid" is noise. v1.1 uses only lanes where the band is present
   (above the SNR floor) at both exposures. Found on case 6.
3. **Per-band half-height.** This clarifies the annotation rule, which already set the
   half-height per band. It was needed for the close doublet of case 6.

**Effect on P1.** Changes 1 and 2 put more pairs and exposures into the endpoints. That
gives more chances to register a QC miss, so the amendment makes the primary criterion
harder to pass, not easier.

The SNR floor, the 0.2 log2 threshold and the 2 px drift limit were **not** changed after
seeing confirmatory data.

### v1.2 (2026-10-25): new primary rule and new confirmatory set

**Why.** Under v1.1, P1 was not met: 4 misses, none with a saturated pixel. The pair-level
"compressive" signature cannot tell which lane of a pair moved. A faint lane that is
under-measured at the short exposure looks like a bright lane compressed at the long one.

**Rule designed on the data.** These rules were designed by looking at the four v1.1 misses
and the six v1.1 series. All six are therefore the **v1.2 development set**, and the v1.2
criterion is judged only on new series.

**1. Lane-level attribution (replaces the pair rule for P1).**
* **Comparison.** For each band, each usable non-reference exposure is compared with the
  reference. Take the lanes above the SNR floor at both exposures with positive signal,
  under the spec's default branch.
* **Lane scale.** Lane i's scale is d_i = log2(S_i at the longer / S_i at the shorter
  exposure) minus the median of the same quantity over those lanes. The median absorbs
  exposure time and substrate decay, so separate captures are handled too.
* **Attribution needs ≥ 3 such lanes.** With fewer, the comparison is not attributable and
  enters only the pair-level endpoints.
* **Classification:**
  * **Sub-linear (bright):** d_i < −0.2 *and* the lane's window reaches ≥ 50 % of the
    detector maximum at the longer exposure. This is detector-like compression, blamed on
    the longer exposure.
  * **Low-signal distortion (S4):** any other |d_i| > 0.2 (super-linear, or sub-linear in a
    dimmer lane), blamed on the shorter exposure. Comparisons whose longer exposure carries
    a saturation flag are left out of S4: their median lane is itself compressed.
* **Why 50 %.** It is physical, not fitted: CCD and CMOS response stays linear well past
  half of full scale. But it was set after seeing two faint lanes (SNR ≈ 11, peak < 30 %)
  come out sub-linear.
* **P1 miss:** an exposure blamed for a sub-linear bright lane that has no lane flagged by
  the spec's saturation rule. **Criterion: zero misses** in the v1.2 confirmatory set (G3
  clause, unchanged).

**2. Candidate near-saturation rule (reported, not judged).**
* A lane is near saturation when its band window holds a pixel ≥ 75 % of the detector
  maximum.
* P1 is also reported with this flag added to the saturation rule.
* τ = 0.75 was chosen after seeing PLC r404 lane 3 reach 79 % of full scale. It is a
  hypothesis for the new series and is **not** part of the spec. Promoting it to the spec,
  as a warning or an exclusion, is a separate decision to be taken after the data.

**Unchanged:** spec v1.2.0, SNR floor 10, the 0.2 log2 threshold, 2 px drift, annotation
rules, P2 and S1–S3. The v1.1 pair-rule P1 is also reported for comparison.

**The four v1.1 misses under v1.2 (development):**

| v1.1 miss | v1.2 |
|---|---|
| GTSF-1 B `lower`, 150 s | 4.5 s auto capture blamed for low-signal distortion: lane 2 d = +0.48 (SNR 24); lane 6 d = −1.03 with peak 25 % of full scale (SNR 11). Not a miss. |
| PLC `r404`, 20 s | no lane beyond ±0.2 (lane 3 d = −0.16, peak 79 %); the candidate rule flags the exposure. Not a miss. |
| PLC `r444`, 20 s | no lane beyond ±0.2 (largest d = +0.20). Not a miss. |
| GFP `doublet`, 295 s | 20 s exposure blamed for low-signal distortion: lane 9 d = +0.54 (SNR 15). Not a miss. |

Development result under v1.2: 0 misses (spec rule), 0 (candidate rule), 3 exposures with
low-signal distortion. **This is not evidence**: the rule was built on these series.

**Annotation note.** In `plcg_ip_2025/flag_right` the lane-3 tubulin window contains a
saturated 13x13 px dust spot at 20 s (y ≈ 647). The QC flags it, as it should. The lane is
below the SNR floor and enters no pair. The annotation is left unchanged.

**v1.2 confirmatory candidates.** Studies from the EMBO SourceData scan not screened
before, in this order:

1. S44318-026-00772-6 (dY-α-tubulin long/short, raw)
2. S44319-025-00593-4 (anti-FLAG high/low exposure)
3. S44319-025-00590-7 (ASYM26 long/short)
4. S44318-025-00548-4 (pellet WB high/low, raw)
5. S44319-026-00773-W (GNPTAB-Myc short/long)
6. S44319-025-00455-Z (HA higher/lower exposure)
7. S44321-026-00433-5 (APEX high/low)
8. S44318-026-00753-9 (UFM1 lower-exposure replicates and their higher exposures)
9. S44318-026-00783-3 (SCH 0 s / 8 s / 30 s / 150 s)
10. S44318-026-00814-Z (`.scn` acquisitions)
11. S44319-025-00558-7 ("longer exposure" files and their shorter partners)
12. S44318-025-00577-Z (HA long/short)
13. S44319-025-00561-Y (auto- and over-exposed)
14. S44319-026-00828-Y (high-sensitivity westerns)
15. S44319-026-00727-2 (chemiluminescence 10 s / 15 s)
16. S44318-026-00889-8 (10 s / 180 s / 5 s westerns)

Screening is done in two passes:

* **Pass 1, every pair:** format, dimensions and phase-correlation shift.
* **Pass 2, survivors only:** previews and annotation.

Every candidate is reported in `validation/exposure/screening.md`.
