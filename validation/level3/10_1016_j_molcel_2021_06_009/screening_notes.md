# Screening notes: 10.1016/j.molcel.2021.06.009

Neguembor et al., "Transcription-mediated supercoiling regulates genome folding and loop
formation", Mol Cell 81 (2021). Deposit: Mendeley Data pbnc4zhcwd v1.

## Sources

* Article: PMC9482096 (author manuscript), `article/PMC9482096.html`; Europe PMC record
  `article/europepmc_search.json`.
* Supplementary information (figures and legends S1 to S7):
  `https://ars.els-cdn.com/content/image/1-s2.0-S1097276521004561-mmc1.pdf`, saved as
  `article/mmc1_supplementary.pdf` (SHA-256 0ac12ac30157ec65087dc668009b11309d3ffa28ad69b92a9648c95139aff9a7);
  rendered Figure S1 page in `article/figure_S1_page.png`.
* Deposit archives in `archives/`. Western blot archives in the deposit:
  `Figure S1A Cell cycle WB.zip` (SHA-256 8a4cce3e...2c5a23) and
  `Figure S6A SMC1A & SMC3 kd WB.zip` (SHA-256 bc7f9b8f...cdf5043, downloaded during screening).
  No xlsx/csv/source-data file exists in the deposit.

Criterion 1 passes: peer-reviewed article; STAR Methods: "Quantification of western blots was
performed by Image Studio Lite 5.2 software (LI-COR), protein levels were normalized to
Vinculin." Main Figures 1-7 contain no western blot (super-resolution imaging, ChIP/RNA-seq,
confocal); the quantified western blot panels are in the supplement.

## Panels in figure order

| Panel | Result | Evidence |
|---|---|---|
| S1A/S1B (Cdk2 pTyr15 and H3 pSer10 / vinculin, Prolif vs Arrest HeLa) | **criterion 5 failed** | C2 pass: `blotverse inspect` on `S1A_Cdk2phTyr15 WB.tif`, `S1A_H3phSer10 WB.tif`, `S1A_Vinculin WB.tif`: 1408x1024, 16 bit, effective 16 bit, 0 saturated px, raw = yes. C3 pass: whole membrane strips are deposited. C4: legend states "n = 3 independent experiments"; the deposit strips carry 12 sample lanes (two blocks of 6, separated by a marker lane), so enough lanes are present. C5 fails: no lane labels exist in the legend, methods, file names or deposit (the `+Marker.jpg` files are unlabelled overlays; no sample guide is deposited for S1A). The figure shows only a 2-lane representative crop (Prolif, Arrest). The deposit holds 12 lanes for 3 + 3 quantified replicates; which lanes were quantified, how the two 6-lane blocks relate (replicate sets, technical duplicates or different loads; vinculin is visibly weaker in the second block) and the group order are stated nowhere. The only cue to group order is the Cdk2 pTyr15 signal itself (the outcome). The mapping is ambiguous. (C7 would also fail: no per-replicate numbers in the deposit or supplement; only the S1B bar chart.) |
| S6A (SMC1A, SMC3 / vinculin, NS vs SMC1A kd vs SMC3 kd shRNA HeLa) | criterion 4 failed | C2 pass: all four TIFs (`S6A a-SMC1A WB.tif`, `S6A a-SMC3 WB.tif`, `S6A a-Vinculin gel SMC1A WB.tif`, `S6A a-Vinculin gel SMC3 WB.tif`) 16 bit, raw = yes. C3 pass: uncropped membranes with `Sample Guide Uncropped WB.pptx` lane labels. C4 fails: the legend gives densitometry of a single representative blot ("Densitometric quantification of blots is shown above each band normalized by NS"), no n for the western blot; the sample guide shows one lane per group used in the manuscript (NS, SMC1 #17, SMC3 #18). |

## Decision

**Excluded.** First quantified panel in figure order: Figure S1A/B. First failing criterion of
the candidate: **criterion 5** (lane-to-group and replicate mapping of the 12 deposited lanes
is not identifiable without using the band signals; no labels or sample guide for S1A).
No panel qualifies, so no `fetch.sh` or `claim.yaml` was written.

## Rescreen under Amendment 2

Rescreened 2026-10-27 under Amendment 2, point 5. Two rules changed: criterion 7 no longer
excludes, and phospho / total counts as a loading control when each lane can be matched.

* **Phospho / total.** The article has no phospho-to-total normalisation. STAR Methods says "protein levels were
  normalized to Vinculin", and the antibody table lists no total Cdk2 or total H3. The S1
  phospho targets (Cdk2 pTyr15, H3 pSer10) are normalised to vinculin, a housekeeping protein,
  as before. No new panel enters the unit of analysis, and the order is unchanged: S1A/B, then S6A.
* **Criterion 7.** Criterion 7 no longer excludes. As recorded above, the deposit and the supplement hold no per-replicate numbers.

| Panel | First failing criterion | Evidence |
|---|---|---|
| S1A/S1B (Cdk2 pTyr15, H3 pSer10 / vinculin; Prolif vs Arrest) | **5** | Reused from above, because the amendment does not touch it. C2–C4 pass. The 12 deposited lanes (two 6-lane blocks) have no labels, sample guide, legend description or file-name cues, so the mapping of lanes to group and replicate cannot be identified without the band signals. |
| S6A (SMC1A, SMC3 / vinculin) | 4 | Reused from above, unchanged: one representative blot, one lane per group. |

**Decision under Amendment 2: still excluded, criterion 5** (Figure S1A/B, the first quantified panel).
No `fetch.sh` or `claim.yaml` was written.
