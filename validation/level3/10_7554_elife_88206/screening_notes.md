# Screening notes: 10.7554/elife.88206

Candidate: Castano et al. 2023, *Discovery and characterization of a specific inhibitor of
serine-threonine kinase CDKL5 (CAF-382/B1) demonstrates role in hippocampal CA1 physiology*,
eLife 12:RP88206. Version of record, PMC10406435.
Deposit: Harvard Dataverse doi:10.7910/DVN/FRRMYI (README in `article/dataverse_FRRMYI_README.md`).
The article's own eLife source-data zips (`elife-88206-fig*-data1.zip`) were also checked.
They hold the same labelled PNGs as the Dataverse zips, plus the uncropped 16-bit TIFFs.

Article files are in `article/`: JATS XML from Europe PMC, the PDF, the figure JPGs and the deposit README.

## Criterion 1

Passes. The paper is a peer-reviewed eLife version of record, and it has western blot panels that the authors quantify (Fig. 5, Fig. 5—figure supplement 1, Fig. 6B).

## Panels that fit the unit of analysis, in figure order

The unit of analysis requires a target normalised to a **housekeeping protein or total-protein stain**. The methods say: "EB2 phosphorylation was measured relative to total EB2 and catenin phosphorylation to total catenin. The other proteins were normalized to tubulin." So only Fig. 5—figure supplement 1 (protein/tubulin) qualifies as a quantified panel.

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 5—S1A: CDKL5/tubulin, CAF-382 (B1), 0/5/50/500/5000 nM | **4** | Criterion 2 passes on the eLife TIFFs: `Western blot 1 - B1 - CDKL5 - uncropped.tif` and `Western blot 6 - B1 - tubulin - uncropped.tif` are both 16-bit, `raw` = yes, 0 saturated px. The Dataverse zip holds only the labelled PNGs, which are rejected. Criterion 3 passes: the full membrane is shown. Criterion 4 fails. The legend states "N = 3 biological replicates with two repetitions", but each deposited membrane (CDKL5 WB1, tubulin WB6) labels only **one** 5-lane B1 set as "CAF-382 (B1) 0–5000 nM". Two further 5-lane sets on the same membranes are unlabelled, and no other CDKL5 or tubulin B1 image is deposited. So the deposited images cannot be shown to contain every replicate (3 biological × 2 repetitions) for target and loading control. Criterion 5 would also fail, because the unlabelled sets cannot be assigned to group or replicate without the authors' numbers. |
| Fig. 5—S1B: total EB2/tubulin, B1 | 4 | Same membranes and layout: WB2 EB2 and WB6 tubulin, each with one labelled B1 set and N = 3 × 2 stated. |
| Fig. 5—S1C: total β-catenin/tubulin, B1 | 4 | Legend: "N = 2 biological replicates". |
| Fig. 5—S1D, E: CDKL5 and EB2/tubulin, HW2-013 (B4) | 4 | Same as A/B. WB7/8/12 each label one B4 set, and two sets are unlabelled (e.g. WB12 tubulin). |
| Fig. 5—S1F: β-catenin/tubulin, B4 | 4 | N = 2. |
| Fig. 5—S1G, H: CDKL5 and EB2/tubulin, LY-213 (B12) | 4 | Same as A/B (WB13/14/18). |
| Fig. 5—S1I: β-catenin/tubulin, B12 | 4 | N = 2. |

## Western blot figures that are not experiments under the unit of analysis

| Panel | Why it is not a unit |
|---|---|
| Fig. 1 | Screening blots, one membrane per compound. No quantification is shown in the figure or legend, and the text gives only qualitative statements. |
| Fig. 4B | "Representative western blot", not quantified. Its raw file `fig 4 source 1.tiff` is also rejected by inspect (pseudo-colour). |
| Fig. 5A–C | pS222 EB2 / total EB2 and phospho-β-catenin / total β-catenin. These are phospho-to-total ratios, not normalised to a housekeeping protein or total-protein stain. If they were counted, Fig. 5A would come first, and it would fail criterion 4 for the same reason: the B1 pEB2 and EB2 membranes WB3 and WB2 have the same single-labelled-set layout. |
| Fig. 5—S2 | Not quantified. Its deposited images are JPEG. |
| Fig. 6A/B | Fig. 6A shows CDKL5 example blots, but no stats are given and no loading control is named. Fig. 6B is pEB2 / total EB2, with immunoreactivity read against an on-gel dilution standard curve, so there is no housekeeping or total-protein loading control. For the record, the Fig. 6 pEB2 file `fig 6 081122 eb2P ... full blot flattened.tif` is 16-bit, `raw` = yes, has 221 790 saturated px, and looks inverted. |
| Fig. 6—S1 | SNS-032 pEB2. Normalised as in Fig. 6 ("Compare to Figure 6"), and no raw images are deposited. |

## Decision

**Excluded, criterion 4.** The first quantified panel in figure order is Fig. 5—figure supplement 1A (CDKL5/tubulin, CAF-382). The authors state N = 3 biological replicates with two repetitions. The deposited membranes identify only one 5-lane replicate set per compound, so the presence of every replicate cannot be established.

Two caveats:

1. **Raw images from eLife.** Criterion 2 was judged on the eLife-hosted source-data TIFFs, treated as the article's own deposit. If only the Dataverse deposit counted, criterion 2 would fail first, because the Dataverse Fig. 5 zip contains only labelled RGB PNGs.
2. **Criterion 7 not reached.** The Dataverse deposit has no per-replicate quantification table for Fig. 5 or Fig. 5—S1; there are tables only for Fig. 4 (pzfx) and Fig. 6 (csv). Criterion 7 was not needed for the decision.

No `fetch.sh` or `claim.yaml` was written, because no panel qualified.

## Rescreen under Amendment 2

Rescreened 2026-10-27 under Amendment 2, point 5. Two rules changed: criterion 7 no longer
excludes, and phospho / total now counts as a loading control when each lane can be matched.
Evidence the revised rules do not touch is reused from above: criterion 1, the status of Fig. 1, Fig. 4B and
Fig. 5—S2 as unquantified, and the Fig. 5—S1 findings.

**Effect of the revised rules on panel order.** Fig. 5A–C (pS222 EB2 / total EB2 and
phospho-β-catenin / total β-catenin) now meet the unit of analysis. Each compares several
doses against 0 nM, uses a Kruskal–Wallis test and gives significance marks. Fig. 5A therefore
becomes the first quantified panel in figure order, ahead of Fig. 5—S1A. Fig. 6B (pEB2 / total
EB2) also becomes a unit, but it comes later.

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 5A: pS222 EB2 / total EB2 (and p-β-catenin / total β-catenin), CAF-382 (B1) 0/5/50/500/5000 nM | **4** | **C2 passes.** The eLife source data `elife-88206-fig5-data1-v2.zip` (https://cdn.elifesciences.org/articles/88206/elife-88206-fig5-data1-v2.zip, SHA-256 9722667fa2aeaea852edf11dc9c249b7d8fd022f620944fe6173c7deebe92b4e, downloaded to scratch only) contains `Western blot 2 - B1 - EB2 - uncropped.tif` and `Western blot 3 - B1 - pEB2 - uncropped.tif`. `blotverse inspect` reports both as 1024x1408, 16-bit, effective 16 bits, 0 saturated px, raw = yes. **C3 passes**: whole strips are shown. **C4 fails.** The legend states "n = 3 biological replicates with two repetitions". Only one pEB2 membrane (WB3) and one total-EB2 membrane (WB2) are deposited for B1. Each holds three 5-lane sets separated by marker lanes, and only one set per membrane is labelled "CAF-382 (B1) 0 5 50 500 5000". The labelled set is not the same on the two membranes: it is the 1st set on pEB2 WB3 and the 2nd set on total-EB2 WB2. The other sets carry no labels. No image of the second repetition is deposited. So the deposited images cannot be shown to contain every quantified replicate for both the target and the reference band. This is the same finding as Fig. 5—S1A above. For the record, C5 would also fail, together with the new phospho/total condition that each lane can be matched. Assigning the unlabelled sets to replicates, and pairing pEB2 lanes with total-EB2 lanes across two separately run membranes whose labelled sets differ, would need the band signals. C7 (no longer excluding): no per-replicate table for Fig. 5 exists in the Dataverse deposit or the eLife source data, which hold images only. |

The later panels are not re-examined because the stop rule applies at the first qualifying panel and none qualified. Fig. 5—S1A–I (criterion 4) is unchanged by the amendment. Fig. 5B/C (B4, B12) have the same layout as Fig. 5A: one labelled set per membrane (WB7–12, WB13–18) and N = 3 × 2 stated. Fig. 6B is now a unit (pEB2 / total EB2), but no raw image of total EB2 is deposited for it. The Fig. 6 deposit holds only the pEB2 `full blot flattened.tif`.

**Decision under Amendment 2: still excluded, criterion 4.** The first quantified panel in figure order is now
Fig. 5A (pEB2 / total EB2, CAF-382), which comes ahead of Fig. 5—S1A. The criterion and the reason do not change. No `fetch.sh` or
`claim.yaml` was written.
