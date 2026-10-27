# Screening notes: 10.1016/j.chom.2018.11.008

Laventie et al., "A Surface-Induced Asymmetric Program Promotes Tissue Colonization by
Pseudomonas aeruginosa", *Cell Host & Microbe* 25:140-152 (2019). Second frame, rank 8.
Deposit: Zenodo 1424619 ("Supplemental data Fig 4A"). Screened 2026-10-27 under the Brief
with the Amendment 2 addition.

## Sources

* No PMC full text (Europe PMC: PMID 30581112, no PMCID). Elsevier open-archive files:
  * `article/1-s2.0-S1931312818305900-mmc8.pdf` = article plus supplemental information
    (`https://ars.els-cdn.com/content/image/1-s2.0-S1931312818305900-mmc8.pdf`, SHA-256
    d74af6bad65aacec2cbf6aab54ff4cd458d51466e6923e2380c9a704e93b8963); text extract
    `article/mmc8_text.txt`; rendered Figure 4 page `article/figure4_page.png`.
  * `article/1-s2.0-S1931312818305900-mmc1.pdf` = Document S1, Figures S1-S7 (SHA-256
    7aac69300a3310fdb9c91d3e59c4f4afc5faad613acc5944ff01cb6435d17524).
* Deposit record from the Zenodo API: `article/deposit_metadata.json`. The deposit holds
  four files, all for Fig. 4A: whole-cell lysate and purified-pili anti-PilA blots, each
  as a "raw_image" and a non-raw TIFF. `images/Fig4A_Western_Blot2_purified_pili_raw_image.tif`
  was downloaded by `screen_files.py` (`blotverse inspect`: 16-bit, raw = yes).

## Criterion 1

Peer-reviewed article, but **no western blot panel is quantified by the authors**.

* The only western blots in the article are Fig. 4A (anti-PilA on whole-cell lysate and on
  sheared surface pili of WT, ΔfimW, ΔfimX, ΔpilA; legend: "Mutants lacking FimW or FimX
  express normal levels of PilA but are unable to assemble T4P") and Fig. S1C (anti-Flag
  blot confirming the size of FimW-Flag). Both are shown as single representative images
  with no densitometry, no normalisation to a loading control, no n and no statistics.
* The STAR Methods "Piliation Assay" describes the immunoblot and ECL detection on a
  LAS4000 but no quantification; "Quantification and Statistical Analysis" covers
  microscopy, TEM, motility and attachment only.

| Panel | Result | Evidence |
|---|---|---|
| Fig. 4A (PilA, whole cell / sheared pili) | criterion 1 failed | qualitative blot, no quantification in the figure, legend, text or methods; no loading control shown |
| Fig. S1C (FimW-Flag) | criterion 1 failed | qualitative size check, not quantified |

## Decision

**Excluded.** First failing criterion of the candidate: **criterion 1** (no western blot
panel quantified by the authors). No `fetch.sh` or `claim.yaml` was written.
