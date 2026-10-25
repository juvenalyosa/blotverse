# Image-integrity screening: validation protocol

Status: **v1.0**, fixed before any confirmatory image was screened or used in the injection
bench. The detector settings are the defaults of `ForensicsOptions`
(`include/blotverse/forensics/forensics.hpp`) at the commit that adds this file.

## What is being validated

`blotverse forensics` screens raw blot acquisitions for regions a person should review:

* **duplicated regions** (`copy`): a region whose raw pixels equal another region's, up to
  brightness/contrast, flips and 180-degree rotation, within one image or across images;
* **same scene** (`same_scene`, across images only): two files that are captures of the
  same membrane, recognised from shared fine texture and fixed-pattern noise;
* **straight seams**: a one-pixel step along a column or row, sustained for at least
  128 px, as a pasted strip from another acquisition leaves;
* **flat regions**: blocks with far less noise than the image's quietest blocks, as a
  painted or filled area leaves.

A finding is never evidence of misconduct on its own; the report says so.

## Method in brief

* **Residual.** Image minus a separable 5-point quadratic Savitzky-Golay smoothing,
  clipped at 3 local robust noise levels.
* **Patches.** 12 x 12, compared after double centring: each patch loses its own row and
  column means. That removes CCD row/column read-out noise and lane or band edges, using
  the patch alone, so a pasted region and its source are centred identically.
* **Search.** Locality-sensitive hashing of the signs of the centred residual (8 tables x
  24 bits) proposes candidate patch pairs in every orientation. Candidates with residual
  correlation >= 0.5 are kept.
* **Copy test (decisive).** A least-squares fit `target = g x source + c` on the raw
  pixels must leave an error, beyond the rounding a gain change produces, of at most
  **0.1 x the local camera noise**. Two genuine regions, or two captures of one membrane,
  each carry their own noise and differ by about sqrt(2) x noise. A digital copy differs
  only by rounding.
* **Regions.** Accepted pairs along one mapping are grown over 4-connected patch positions.
  A copy is reported from 16 accepted positions upward.
* **Same scene.** Across images only, the three most-seeded mappings of each image pair and
  orientation are grown at correlation >= 0.5. The pair is reported (once) when the region
  covers >= 10 % of the smaller image.
* **Seams.** Per 64-row (or 64-column) segment, the median step across each column
  boundary is compared with all others (robust z >= 10). The step across the boundary must
  carry 0.8-1.25 of the step across 6 px (a one-pixel step, not a blurred edge).
* **Flat.** 8 x 8 windows (half-overlapping) with residual noise below 0.25 x that of the
  quietest decile of windows. Windows with saturated pixels or a quarter at the image minimum are
  excluded.

## How the settings were reached (development, exploratory)

The development images are the exposure-bench development set: GTSF-1 (11 `.scn`), GFP-Set1
IP (2 TIFF) and PLCγ/FLAG IP (4 TIFF). Each failure seen there changed the method:

| Failure on development images | Change |
|---|---|
| 2100 matches in one image pair: lane edges and band edges shifted along themselves | peak-sharpness test, then line detrending (later replaced) |
| Matches driven by single bright specks | residual clipped at 3 local noise levels |
| PLC/FLAG: row read-out noise and resampling ringing gave correlation 0.65-0.95 between unrelated regions | the copy decision moved from correlation to the raw-pixel error test |
| Dark regions passed the error test against the image-wide noise | local patch noise |
| GTSF-1 cumulative frames (later frames contain earlier photons) passed at 0.25-0.5 x noise | copy threshold 0.1 x noise; these pairs are now `same_scene` |
| Seams at blurred lane edges | share window 0.8-1.25 and 128-px minimum length |
| Flat findings in dark, low-shot-noise areas | reference noise = quietest decile of blocks |
| Injection bench 1: copies missed (0.78-0.87) because line detrending mixed in the surroundings, which differ between copy and source | double centring inside each patch instead |
| Injection bench 1: erasures never found (0/45): 16-px aligned blocks rarely fit inside a 16-32 px erased area | 8 x 8 sliding windows |
| Injection bench 2: copies 16-17 px tall missed: the copy's border contaminates every 16 x 16 patch | patch 12 (bench 3: copies 0.98-1.00, no added false findings) |
| Patch 12: the FLAG 5 s/20 s pair covered < 20 % of the frame (the membrane fills only part of it) | same-scene area 10 % (unrelated-image regions in development stayed below 1 %) |

The development injection bench (`validation/results/forensics/development/`, seed 1) was
rerun with the final settings. Its result is reported but is not evidence: the settings were
chosen on the same images. In development the seam test also fired on 2 of the 9 untouched
bench images (a straight membrane edge and a saturated lane edge).

## Confirmatory sets

Images never screened before this file was committed:

* **UFM1** (EMBO J 2026, doi:10.1038/s44318-026-00753-9): 8 ChemiDoc TIFFs, 4 membranes x
  2 exposures (`validation/exposure/ufm1_2026/`).
* **AURORA A** (level-3 case, `validation/level3/aurora_a_2026/`): 6 TIFFs, HA and DICER x 3
  replicates.
* **Pillai-Kastoori 2020** LI-COR dilution series: 2 TIFFs, 700 and 800 nm channels of one
  membrane (`validation/real_data/pillai_kastoori_2020/`).

The UFM1 and AURORA images had been measured before, but never screened for integrity.

## Endpoints

**E1. False copies on untouched images (primary).** Every confirmatory study is screened
with all its images together (within-image and cross-image search).

* Criterion: **0 `copy` findings**.
* Every other finding is listed and examined; seams and flat regions are reported with
  their likely cause.

The deposited images are assumed genuine. If a `copy` finding nevertheless appears, it is
examined, reported factually and counted as a failure of E1. It is never reported as an
allegation.

**E2. Same-scene recognition.** Expected `same_scene` pairs:

* the 4 UFM1 normal/lower-exposure pairs that the exposure screening found aligned;
* the Pillai-Kastoori 700/800 pair (same membrane, two channels): an expectation, not a
  criterion, because two fluorescence channels may share little texture.

A `same_scene` finding between AURORA HA and DICER of the same replicate is plausible
(re-probed membrane) and is reported without judgement. Any other `same_scene` finding is a
false match.

**E3. Sensitivity (injection bench, seed 2).** `blotverse forensics-bench` on all
16 confirmatory images, 5 repeats per manipulation (80 trials per manipulation; splice needs a
donor at least as tall, so it may have fewer):

* Criterion: detection rate **>= 0.9** for `copy`, `copy_gain`, `copy_flip` and
  `cross_copy` (point estimate; the Wilson 95 % interval is reported).
* `splice` and `erase` are reported without a criterion: they are the lower-confidence
  tests.
* `copy_noise` (0.5 x noise added) lies outside the copy test's scope. It is reported to
  show the limit; detection near 0 is expected.

**E4. False findings in the injection bench.**

* Criterion: in `none` trials, **0 false `copy` findings**.
* For each manipulation, the fraction of trials with any false finding is reported, with its
  Wilson interval.

## Limitations stated in advance

* Copies that are rescaled, rotated by angles other than 180 degrees, blurred, re-noised,
  or JPEG-compressed are outside the copy test.
* Figures from PDFs have lost the noise this method relies on.
* The seam test cannot tell a pasted strip from a straight, sharply cut membrane edge. In
  development it fired on such an edge and on the edge of a saturated lane.
* Synthetic manipulations are simple: copies are pasted without blending at the borders,
  splices are full height, and erasures use a constant fill. Skilled manipulation can be
  harder.
