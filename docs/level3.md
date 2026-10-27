# Level 3: reanalysis of published western blots

Plan section 10 calls for reanalysing published experiments whose raw, uncropped images
and the authors' own quantification are available.

**Inclusion criteria** (plan section 10):

* raw, uncropped image;
* at least 3 biological replicates per group;
* lanes and groups identifiable from the article or its metadata;
* the authors' quantification published.

**Two checks per experiment:**

* **Technical:** does BlotVerse measure the same lanes as the authors' software?
* **Scientific:** does the authors' conclusion survive the multiverse?

Status: pilot. Two experiments have been processed by one annotator. The plan requires two
independent annotators and 30–50 experiments; the confirmatory study follows
[level3_protocol.md](level3_protocol.md), and these pilot cases are not part of it.

## Case 1: AURORA A – DICER, Fig. 3G (included, partially)

* **Article:** EMBO Reports 2026, doi:10.1038/s44319-026-00850-0.
* **Data:** EMBO SourceData S-SCDT-10_1038-S44319-026-00850-0, CC BY 4.0. Six raw 16-bit
  TIFFs (Amersham ImageQuant 800) and the authors' ImageQuant signals per lane.
* **Experiment:** HA-IP of AURORA A WT, mut1 and mut2. Co-precipitated DICER is normalised
  to HA, n = 3.
* **Authors' claim:** both mutants lose DICER binding.

**Lane mapping.** The authors' overview PDF identifies the lanes: input x4, then HA-IP
control, WT, mut1, mut2.

**Excluded:**

* **HA of replicate 3.** The deposited HA_R3 image holds four strips and the files do not
  say which one is replicate 3. Choosing the strip that best matches the authors' numbers
  would bias the comparison.
* **Input lanes.** They are saturated in DICER_R1 (pixels at 65535, flagged by BlotVerse's
  QC), but the authors did not quantify them.

**Measurement.** Lane and band positions were located visually and refined from the image
profiles with `validation/level3/refine_lanes.py`. DICER and HA are in separate images; this
case led to the new `reference_image` support (a loading control imaged separately, with
its own lane geometry).

Reproduce everything with `validation/level3/aurora_a_2026/run.sh`; results go to
`validation/results/level3_aurora_a_2026/`.

### Technical check: BlotVerse vs the authors' ImageQuant values

Fifteen lane measurements: DICER in R1–R3 and HA in R1–R2.

| Branch | r (log signal) | Median relative difference | log2 FC mut1/WT, R1 · R2 (authors) | log2 FC mut2/WT, R1 · R2 (authors) |
|---|---|---|---|---|
| rb1/rect | **0.966** | **24 %** | −1.20 · −4.45 (−1.33 · −3.14) | −1.35 · −6.03 (−1.28 · −5.01) |
| rb2/rect | 0.940 | 26 % | −1.05 · −3.90 | −1.05 · −5.63 |
| rb1/mask | 0.974 | 56 % | −0.70 · −4.56 | −0.68 · −6.14 |
| rb4/mask | 0.778 | 41 % | **+0.81** · −3.75 | **+0.51** · −5.05 |

The table shows 4 of the 12 branches; all of them are in `comparison.md`.

* **BlotVerse reproduces the authors' measurements.** The closest branch to ImageQuant's
  rectangle-minus-background (`rb1/rect`) correlates at r = 0.97, and its fold changes are
  close to the authors' in every replicate.
* **In replicate 1 the mutant bands are close to background.** The large rolling ball
  (`rb4`) with an Otsu mask turns the effect positive there: a single replicate's
  conclusion depends on how background is handled.

### Scientific check: the multiverse

Replicates 1 and 2 enter the multiverse: 48 universes once the spec's design rules are
applied.

| | mut1 vs WT | mut2 vs WT |
|---|---|---|
| Direction | **48 / 48 universes negative** | **48 / 48 negative** |
| log2 FC range (5–95 %) | −4.3 to −1.0 (full range −4.3 to −0.1) | −5.4 to −1.2 (full range −5.4 to −0.5) |
| Most influential decision | band definition (η² = 0.60) | effect scale (η² = 0.47) |
| Significant universes | 0 of 48 (n = 2) | 0 of 48 (n = 2) |
| Verdict | Not evaluable (2 blots x 1 lane per group) | Not evaluable |

**Interpretation:**

* The direction of the authors' conclusion is robust to every defensible analysis.
* Its size is not: from about 1.1-fold to 40-fold.
* With the two replicates that the deposited files allow us to reconstruct, no analysis
  reaches significance. The authors reported n = 3, but the third replicate's loading
  control cannot be identified from the deposit.

## Case 2: UFMylation and RQC, Fig. 3J/K (excluded)

* **Article:** EMBO J 2026, doi:10.1038/s44318-026-00753-9, PMC13144351.
* **Data:** EMBO SourceData S-SCDT-10_1038-S44318-026-00753-9 (Bio-Rad 16-bit TIFFs) and
  the authors' signals.
* **Experiment:** UFMylated RPL26 normalised to eEF2, AAVS1 vs ANKZF1 knockout, n = 3. The
  authors report no significant difference.

**Excluded: lanes not identifiable.** The deposited membranes have 12 lanes, the published
panel J shows two cropped lanes, and neither the article nor the metadata say which 6
lanes are the 3 replicates. Inferring the mapping from the authors' numbers would be
circular.

**QC observation.** It is not part of the analysis because the case is excluded. The
deposited image that most likely shows the eEF2 loading control has **940 pixels at the
detector maximum** along its single band, across nearly all lanes. The UFMylated RPL26
bands that were quantified have none. If that exposure was used for normalisation, the
loading control was outside its linear range; BlotVerse would flag those lanes and exclude
them.

## What real data taught the tool

* **Loading controls are often imaged separately** (re-probed, cut strips, other
  exposures). The experiment format now has `reference_image` and
  `reference_annotation`. A test checks that it gives identical results when the "other"
  image is the same image.
* **Deposited files must be checked, not assumed.**
  * The search agent flagged ImageQuant 800 TIFFs as stored inverted; the data show they are
    not.
  * Strips, multiple exposures and unlabeled lanes are common.
* **Saturation is common in published raw images.** BlotVerse's QC found it in both
  studies.

## Next candidates

These were found and verified by header checks (see real_data.md):

* **SMG1:SMG8:SMG9 NMD** (NAR 2026, Zenodo 18299536): raw uint16 chemiluminescence and
  stain-free total-protein `.scn`, with UPF1 / pUPF1 / TCE volumes for 3 replicates. It
  would also exercise the total-protein loading branch.
* **Adiponectin / exosomes** (Mol Ther 2020, Zenodo 4598136): about 90 native `.scn` files
  with Image Lab lane definitions embedded, and the authors' volume export.
* About 20 further EMBO SourceData studies with 16-bit blots, to screen against the
  inclusion criteria.
