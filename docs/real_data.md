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

## 4. Known truth on a real image: Pillai-Kastoori et al. (2020) dilution series

**Source.** Pillai-Kastoori L. et al. (2020), *A systematic approach to quantitative
Western blot analysis*, Anal Biochem 593:113608. Data: Mendeley Data
doi:10.17632/vwcy56z5jh.1, CC BY 4.0.

**The image.** A raw LI-COR Odyssey CLx acquisition (16-bit floating point, now read
natively) of one lysate loaded at 40, 30, 20, 10, 5, 2.5 and 1.25 µg. The lane loads come
from the authors' Image Studio annotations. Four bands on the 800 nm channel are labelled
in the authors' annotation as cPARP, β-actin, GAPDH and COXIV; the channel assignment of
the labels was not verified.

**Reproduce.**

```bash
validation/real_data/pillai_kastoori_2020/fetch.sh
build/apps/blotverse dilution validation/real_data/pillai_kastoori_2020/dilution_800.yaml --out OUT
```

`blotverse dilution` quantifies every band under every background x band-definition
branch of the spec (12 combinations). For each combination it reports:

* the log-log slope of signal against load;
* the lanes the linear-range QC rule flags;
* the error of the estimated log2 fold change between lanes, whose true value is known
  from the loads.

Results are in `validation/results/dilution_pillai_kastoori_2020/`.

| Band | Proportional range found (all 12 branches unless noted) | Known-FC error inside it (log2) | Known-FC error over all pairs (log2) |
|---|---|---|---|
| β-actin | 5–20 µg | **0.09–0.13** | 1.88–2.21 |
| COXIV | 2 loads only; *which* 2 depends on the branch (5–10 or 20–30 µg) | 0.06–0.10 | 1.09–1.46 |
| GAPDH | none (profile-peak branch: 2 loads) | – | 1.11–1.52 |
| cPARP | none | – | 1.73–2.39 |

Findings:

* **Inside the proportional range, BlotVerse recovers known fold changes accurately on a
  real blot:** within 0.13 log2 for β-actin in every branch.
* **Outside it, no analysis branch can rescue the data.** Errors of 1–2.4 log2 come from
  overload at 30–40 µg (the 40 µg lane is visibly distorted) and from loss of signal at
  2.5–1.25 µg.
* **Branch choice moves even the apparent linear range** (COXIV) and changes the
  all-pairs error by up to 0.6 log2 (cPARP).
* **Effect on the QC rule.** The original rule only caught the flattening at high loads.
  Spec v1.2 makes it two-sided: slopes outside [0.8, 1.25] mark a lane. The rule is now
  also applied in the pipeline to any blot whose lanes declare a `load`, as plan section 5
  requires. Level-1 results are unchanged: seed-1 records are byte-identical under v1.2.

**Limits.** This is one membrane with one replicate per load. It tests linearity and
fold-change recovery, but not replicate statistics or the robustness score. Those still
need the level-2 design (3 gels per condition).
