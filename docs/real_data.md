# Real data: availability, realism check and lane detection

## 1. Public raw western blot data (survey of October 2026)

**No public dataset combines raw 16-bit blot images with known loading amounts or a known
spike-in fold change.** Level 2 of the plan, a laboratory dilution series that would become
BlotVerse-Bench, therefore remains necessary.

Two papers publish ground truth only as numbers, with no raw images: Butler 2019
("Misleading Westerns", PMC6360618, xlsx) and Degasperi 2014 (S9).

Raw 16-bit acquisitions do exist, but without ground truth. Native imager files are truly
raw:

* Bio-Rad Image Lab `.scn`, read natively by BlotVerse;
* ImageQuant LAS `.gel` (Molecular Dynamics TIFF);
* some 16-bit greyscale TIFFs.

Most files deposited as "raw" are 8-bit or RGB exports (see the audit in
[validation.md](validation.md)).

| Dataset | License | Content (verified) |
|---|---|---|
| EMBO SourceData S-SCDT-10_1038-S44319-026-00914-1 | CC BY 4.0 | 32 ChemiDoc `.scn` blot files, including two exposure series of the same membrane (1–200 s) |
| UNC Dataverse doi:10.15139/S3/4ESRC1 | CC0 | ~380 `.scn` + 152 `.mscn`, 16-bit; authors' ROI boxes saved |
| EMBO SourceData S-SCDT-10_1038-S44318-025-00572-4 | CC BY 4.0 | 194 ImageQuant LAS `.gel` (16-bit linear), multi-exposure sets, per-lane band volumes in xlsx |
| Harvard Dataverse doi:10.7910/DVN/YND7FA | CC0 | 15 ChemiDoc `.scn`, 16-bit, Image Lab lane analysis saved |
| EMBO SourceData S-SCDT-10_1038-S44321-025-00317-0 | CC BY 4.0 | `.scn` with full Image Lab lane/band analysis (2160 x 2160 px) |
| EMBO SourceData S-SCDT-10_1038-S44319-025-00521-6 | CC BY 4.0 | 21 `.scn`, 16-bit |
| Zenodo 10050244 | CC BY 4.0 | ~100 16-bit greyscale TIFFs |
| Figshare 10.6084/m9.figshare.4516847 | CC BY 4.0 | 8 ChemiDoc Touch `.scn` |
| Zenodo 13752216 | CC BY 4.0 | 16-bit TIFF, full-range values (possibly contrast-stretched) |

## 2. Realism check of the simulator (plan section 8)

`blotverse imagestats` computes annotation-free statistics for real and simulated images
alike:

* background (median);
* noise (MAD);
* peak SNR, i.e. (p99.9 − median) / noise;
* background variation: the spread of a smooth rolling-ball background relative to the
  signal;
* saturated fraction.

Sample: 18 real 16-bit acquisitions from the 9 datasets in the table above:
12 `.scn`, 3 `.gel` and 3 TIFF.

| | Peak SNR | Background variation |
|---|---|---|
| Real acquisitions (range; typical) | 25 – 3600; ~350 | 0.001 – 0.13; ~0.02 |
| Simulator v1 "moderate" | ~15 | 0.14 – 0.18 |
| Simulator v1 "severe" | ~5 | 0.46 – 0.64 |
| **Simulator v2 "clean"** | ~590 | 0.0004 |
| **Simulator v2 "moderate"** | ~95 | 0.023 |
| **Simulator v2 "severe"** | ~19 | 0.12 |

The v1 presets had background artefacts that were *much harsher* than anything real.
**Presets v2** were calibrated on these statistics alone, without looking at validation
metrics: moderate and severe background amplitudes were reduced about 4x. The three levels
now span the real range from the best to the worst image.

Two conclusions follow:

* **E1:** the small spread of p-values is not caused by a mild simulator. It stayed at 1–2
  orders of magnitude even with unrealistically harsh artefacts.
* **Saturation:** real long exposures are 0.6–1.2 % saturated pixels (EMBO 00914 at
  50–200 s), and BlotVerse's per-band saturation check excludes such bands.

## 3. Lane detection against authors' annotations

The authors' own Image Lab lanes are the reference. Reproduce with
`validation/real_data/run_lane_check.sh WORK_DIR`, which downloads both files, about 10 MB.

| Blot | Difficulty | Mean / max lane-centre error |
|---|---|---|
| pJNK, Harvard Dataverse YND7FA (12 lanes, pitch 40 px) | ladder lane and dotted membrane edge next to the samples; close doublet band | **1.1 / 2.4 px**, no hints |
| pSTAT1, EMBO 00317 (14 lanes, pitch 88 px) | about half the lanes are blank controls | 225 px without a hint (blank lanes cannot be located) → **3.8 / 22 px** with `lanes_x: [first, last]` |

The 22 px case is a lane the authors placed off the regular spacing by hand.

This check produced three changes to the detection:

* **Lanes:** the first version picked the n strongest column peaks and locked onto the
  ladder and the membrane edge, an error of about 41 px, or one lane. Lanes are now found by
  fitting an evenly spaced comb that maximises lane-versus-gap contrast.
* **Bands:** band search windows no longer overlap when target and reference are close.
  Before, both rows of the JNK doublet collapsed onto the same band.
* **Hint:** the optional `lanes_x` hint pins the comb.

Both real cases are now covered by regression tests on simulated equivalents (ladder lane,
doublet).
