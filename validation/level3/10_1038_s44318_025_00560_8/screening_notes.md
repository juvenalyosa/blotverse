# Screening notes: 10.1038/s44318-025-00560-8

Chu et al., "Nuclear paxillin functions as a molecular switch for alternative splicing in
neurons during a critical period of brain development", The EMBO Journal (2025),
PMC12583701. Deposit: BioStudies S-SCDT-10_1038-S44318-025-00560-8 (EMBO SourceData).

## Criterion 1

Peer-reviewed journal article (EMBO J, not a preprint) with western blots quantified by the
authors. Passes.

## Western blot panels and how they are quantified (figure order)

The unit of analysis requires a target band normalised to a **loading control (housekeeping
protein or total protein)**. Most quantified blots in this article are co-IP blots normalised
to the immunoprecipitated bait, which is neither:

| Panel | Quantification (legend / y-axis) | Normalised to a housekeeping or total-protein loading control? |
|---|---|---|
| Fig 1K | importin-β2 in FLAG IP, "norm. to IP'd FLAG", WT vs S119A/S119D/3xPY/AA, n = 3 | No: IP bait (FLAG-paxillin) |
| Fig 2E/F | p-Paxillin S119 / Y118 in FLAG IP, "Norm. to IP'd Paxillin", n = 3 | No: IP bait (phospho/total of the bait) |
| Fig 2G/H | p-Paxillin S119 in FLAG IP, "Norm. to IP'd Paxillin", n = 3 | No: IP bait |
| Fig 4C/D | co-IP'd splicing factors, "norm. to IP'd PXN"; PXN "Rel. to Control", n = 3 | No: IP bait; PXN panel not normalised to a loading control |
| Fig 4E | co-IP'd proteins P7 vs P15, "norm. to IP'd PXN" | No: IP bait |
| Fig 4G | co-IP'd proteins in FLAG IP, "norm. to IP'd FLAG", n = 4 | No: IP bait |
| **Fig EV1C/D** | p-Paxillin S119 and Y118 "norm. to actin" (and "norm. to pan-paxillin"), 12 tissues at P7 and P14 | **Yes: actin (housekeeping)** |
| Fig EV1E | p-Paxillin S119/Y118 "Fold change rel. to E17", n = 3–4 | No loading control stated or shown (CDK5 is plotted as a target) |
| Appendix Fig S1E | p-Paxillin S119 in FLAG IP, mean intensity, n = 3 | No: IP blot, no loading control stated |
| Appendix Fig S6A/B | synaptosome-to-pellet ratios of NR1 variants/PSD95, n = 8 | Fraction ratio (actin/PSD95 called loading controls in the legend); not deposited, see below |

So the first panel in figure order that fits the unit of analysis is **Fig EV1C/D**.

## Panels examined

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig EV1C/D | **2** | The deposit (BioStudies) has source images only for main Figs 1, 2 and 4. The only raw-data file for EV1C is the journal ESM `44318_2025_560_MOESM7_ESM.tiff` ("Figure EV1C Source Data", saved in `article/`): `blotverse inspect` → `raw = no`, "rejected: colour (pseudo-coloured) image". It is an RGB composite of the figure panel plus annotated photos of the pan-paxillin membranes only; no greyscale acquisition of p-Paxillin S119/Y118 or actin exists. (It would also fail criterion 4: one lane per tissue, no replicates, single bars without error.) |
| Fig EV1E | not a unit-of-analysis panel; no deposited raw images (criterion 2) | No loading-control normalisation stated; no source images in the deposit or ESM. |
| Appendix S1E, S6A/B | 2 | No raw images in the deposit or ESM for Appendix figures. |

No panel meets criteria 2–8. Decision: **excluded, criterion 2 (Fig EV1C/D)**.

## Note for the reviewer: Fig 1K (the IP-FLAG files found by the file screen)

The file screen flagged `IP-FLAG_western_FLAG-2.tif` (16-bit). These files belong to Fig 1K,
which is normalised to IP'd FLAG-paxillin (the IP bait), not to a housekeeping or
total-protein control, so under the frozen definition it is not an experiment. The GAPDH
bands in 1K are shown but are not used in the authors' quantification. If the IP bait were
accepted as a loading control, 1K would be the first panel, and it would still look doubtful:

* Criterion 2 is mixed. `blotverse inspect` gives 16-bit/raw = yes for FLAG-2, FLAG-3,
  importinB_GAPDH-1/-2, importinB_p-PaxillinS119_GAPDH-1/-4 and p-PaxillinS119, but 8-bit (raw = no)
  for FLAG-1, importinB_p-PaxillinS119_GAPDH-2 and -3.
* Criterion 5 looks problematic. The FLAG membranes are dated 12/9, and the importin-β2/GAPDH
  membranes 2/4 and 3/4. FLAG-1 shows a different experiment (U2AF65 IP; WT con/Fsk, S119A/D Fsk lanes).
  Nothing in the deposit pairs each importin-β2 replicate with its FLAG replicate.

This is not part of the decision. It is recorded so the reading of "loading control" can be
checked.

## Files

* `article/`: Europe PMC full-text XML, PMC HTML, figure JPEGs (Fig2 = main Fig 1,
  Fig4 = main Fig 2, Fig7 = main Fig 4, Fig1 = EV1, Fig3 = EV2, Fig5 = EV3, Fig8 = EV4,
  Fig10 = EV5; EV figures taken from media.springernature.com, because the PMC CDN serves
  duplicated blobs for some figure numbers), the Appendix and EV-figure PDFs, the EV1C source
  TIFF, and the BioStudies study JSON (the file-to-panel mapping).
* `images/`: left as downloaded by `screen_files.py`; no panel qualified, so it was not
  curated. No `fetch.sh` or `claim.yaml` was written, because the candidate is excluded.
* No file containing the authors' numbers was saved in `<DIR>`.

## Rescreen under Amendment 2

Rescreened 2026-10-27 under Amendment 2, point 5. Two rules changed: criterion 7 no longer
excludes, and phospho / total counts as a loading control when each lane can be matched. Co-IP
normalisation to the immunoprecipitated bait is still not a loading control. Evidence the
revised rules do not touch is reused from above: criterion 1, the per-panel quantification
table, and the criterion-2 result for Fig. EV1C/D (the only raw file is the RGB journal ESM
`44318_2025_560_MOESM7_ESM.tiff`, raw = no).

**Effect of the revised rules on panel order.**

* Fig. 1K, 4C/D, 4E and 4G are co-IP blots normalised to the IP'd bait (FLAG or PXN). They are
  still not experiments.
* Fig. 2E/F and 2G/H quantify p-Paxillin S119/Y118 in FLAG immunoprecipitates "norm. to IP'd
  Paxillin". The target is the phospho form of the bait and the reference is the bait itself,
  so they meet the phospho/total wording and the excluded "normalisation to an
  immunoprecipitated bait" wording at once. Read literally, the reference is an IP'd bait and
  they are not experiments. Either reading gives the same result, because they fail criterion 2
  (below).
* Fig. EV1C/D ("norm. to actin" and "norm. to pan-paxillin") was already a unit and stays the
  first panel under the literal reading.

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 2E/F (examined in case the IP'd bait counts as the total form) | 2 | Downloaded the eight `Revision_Figure_2E_*.tiff` files (p-PXN S119, p-PXN Y118 and FLAG M2 blots of input and FLAG IP, two treatment series) to scratch from BioStudies. `blotverse inspect` on all eight: raw = no, "rejected: colour (pseudo-coloured) image". |
| Fig. 2G/H (same reading) | 2 | `Revision_Figure_2G_WB_with_anti-p-PXN_S119_*.tiff` and `..._anti-GFP_*.tiff`: raw = no, colour. `..._Flag_M2_*.tiff`: raw = no, 8-bit. |
| Fig. EV1C/D (p-Paxillin S119/Y118 / actin or / pan-paxillin) | **2** | Unchanged: no greyscale acquisition is deposited, and the ESM TIFF is RGB (raw = no). The phospho/total reading (pan-paxillin reprobe of the same membrane) does not change this. C7 (no longer excluding): no per-replicate table for EV1 is listed in BioStudies. |
| Fig. EV1E, Appendix S1E, S6A/B | 2 | Unchanged: no raw images deposited. |

**Decision under Amendment 2: still excluded, criterion 2.** The first panel is Fig. EV1C/D
under the literal reading of Amendment 2 (Fig. 2E/F under the alternative reading), and the
criterion is 2 either way. No `fetch.sh` or `claim.yaml` was written. The Fig. 2E/G files were
downloaded to a scratch directory only. No file with the authors' numbers was downloaded.
