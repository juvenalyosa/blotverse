# Image-integrity screening: results

* Protocol: [`forensics_protocol.md`](forensics_protocol.md) (v1.0, fixed in commit 0bf007b
  before any confirmatory image was screened).
* Reproduce: `validation/forensics/run_all.sh`. Results go to `validation/results/forensics/`.

## Summary

| Endpoint | Criterion | Result | Met |
|---|---|---|---|
| E1. `copy` findings on 16 untouched confirmatory images (3 studies) | 0 | **0** | yes |
| E2. Expected `same_scene` pairs (UFM1 normal/lower exposure) | all 4 | **4 / 4**; no other `same_scene` | yes |
| E2. Pillai-Kastoori 700/800 nm channels | expectation only | not matched | - |
| E3. Detection: `copy`, `copy_gain`, `copy_flip`, `cross_copy` | each >= 0.9 | **80 / 80 each** (Wilson 95 % 0.95-1.00) | yes |
| E4. False `copy` findings in untouched (`none`) bench trials | 0 | **0 / 80** | yes |

**Scope of the claim.** The copy test finds digital copies in raw acquisitions: whole or
partial regions, with changed brightness/contrast, flipped, or rotated by 180 degrees, within
one image or between images. It finds none when noise is added to the copy (`copy_noise`,
0 / 80), as stated in the protocol.

## Confirmatory injection bench (seed 2, 16 images, 610 trials)

| Manipulation | Trials | Detected | Rate (Wilson 95 %) | Trials with a false finding | False findings, by kind |
|---|---|---|---|---|---|
| none | 80 | - | - | 10 (0.125) | seam 25 |
| copy | 80 | 80 | 1.00 (0.95-1.00) | 12 | seam |
| copy_gain | 80 | 80 | 1.00 (0.95-1.00) | 10 | seam |
| copy_flip | 80 | 80 | 1.00 (0.95-1.00) | 10 | seam |
| copy_noise | 80 | 0 | 0.00 (0.00-0.05) | 10 | seam |
| cross_copy | 80 | 80 | 1.00 (0.95-1.00) | 40 | flat 21 trials, seam 15, both 4 |
| splice | 50 | 50 | 1.00 (0.93-1.00) | 13 | seam |
| erase | 80 | 50 | 0.62 (0.52-0.72) | 11 | seam |

**Where the false findings come from:**

* Every false finding is a seam or a flat region. None is a duplicate.
* **Seams.** All the seams in `none` trials come from two images, UFM1 `WB_UFM1` and its
  lower exposure. They also appear when those two images are screened untouched (below).
* **Flat regions in `cross_copy` trials.** These lie on the pasted region itself. A region
  from a quieter acquisition is "flat" relative to its new host. That is a real signal of the
  paste, but the pre-specified scoring counts it as false (only `splice` exempts it), and the
  table keeps that scoring.
* **Erasures that were missed.** They come from 6 images (AURORA HA R1-R3, DICER R1 and R3,
  Pillai 800 nm). In those images the quietest windows are already nearly noiseless, so a
  constant fill is not quieter than its reference.

The full table is in `validation/results/forensics/confirmatory/forensics_bench.md`; every
trial is in `forensics_bench_trials.csv`.

## Screening of the untouched confirmatory studies (E1, E2)

| Study | Images | `copy` | `same_scene` | Seams | Flat |
|---|---|---|---|---|---|
| UFM1, EMBO J 2026 | 8 | 0 | 4: the 4 normal/lower pairs, identity mapping, correlation 0.77-0.79 | 5 | 0 |
| AURORA A | 6 | 0 | 0 | 0 | 0 |
| Pillai-Kastoori 2020 (LI-COR) | 2 | 0 | 0 | 0 | 0 |

**The 5 UFM1 seams.** They are row boundaries in `WB_UFM1` (rows 225, 301 and 304) and its
lower exposure (rows 412 and 413). Each spans 138-275 px across adjacent lanes. They are the
sharp horizontal edges of the continuous bands that run across the lanes on this membrane.
They appear at the same edges in both exposures, which is what a physical edge does.
Benign. They show the stated limit of the seam test, which cannot tell a sharp physical edge
from a paste.

**AURORA HA vs DICER.** No `same_scene` finding, so nothing suggests these are re-probed
membranes. The protocol made no prediction either way.

**Pillai-Kastoori 700/800.** The two LI-COR channels of one membrane were not matched. Two
fluorescence channels share little fine texture, and the protocol had flagged this as an
expectation only.

## Development (exploratory; settings tuned on these images)

Images: GFP-Set1 IP (2), PLCγ/FLAG IP (4), GTSF-1 (3) in the bench; all 17 development
files in screening.

| Manipulation | Trials | Detected | Rate |
|---|---|---|---|
| copy | 45 | 44 | 0.98 |
| copy_gain / copy_flip / cross_copy | 45 each | 45 each | 1.00 |
| splice | 25 | 25 | 1.00 |
| erase | 45 | 30 | 0.67 |
| copy_noise | 45 | 0 | 0.00 |

**Screening the untouched development studies:**

* `copy`: 0.
* `same_scene` matched 19 of the 23 pairs of same-size captures of one membrane. The 4
  unmatched pairs all involve the 4.5 s auto-exposure of GTSF-1 series B, which has too
  little signal to share texture.
* The exposure ratio comes out of the fit: GFP 20 s vs 295 s gain 0.0675 ≈ 20 / 295.
* Frames of a cumulative acquisition share most of their photons. They differ by 0.2-0.5 x
  noise and are reported as `same_scene`, not `copy`.

## What this means

* **Copy-paste in raw files.** The tool finds it reliably: the same region of a 16-bit
  acquisition appearing twice, in one file or across files, including with brightness or
  contrast changed and flipped. On real images without manipulation it reported no false
  copy.
* **Same blot, different files.** It also recognises when two files are images of the same
  membrane. That is how a reviewer finds an exposure of one blot presented as another
  experiment, and it is the expected result for declared exposure series.
* **Seams and flat regions are prompts, not results.** Seams catch every pasted strip in the
  bench but also fire on sharp physical edges. Flat-region detection finds about 2 in 3
  painted areas.
* **Not covered.** Copies with added noise, rescaling, blurring or JPEG compression, and
  figures taken from PDFs.

## Limitations

* **Small sample.** 3 confirmatory studies and 16 images. 80 trials per manipulation give a
  95 % lower bound of 0.95 on the detection of each copy type.
* **Synthetic manipulations are simple.** Copies are pasted without blending at the borders,
  splices are full height, and erasures use a constant fill.
* **One annotator, not blinded** (the author of the method).
