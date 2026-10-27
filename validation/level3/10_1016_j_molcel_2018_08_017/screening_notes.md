# Screening notes: 10.1016/j.molcel.2018.08.017

Lykke-Andersen et al., "Box C/D snoRNP Autoregulation by a cis-Acting snoRNA in the NOP56
Pre-mRNA", *Molecular Cell* 72:99-111 (2018). Second frame, rank 3. Deposit: Mendeley Data
5sb8j2mypg v1 (10.17632/5sb8j2mypg.1). Screened 2026-10-27 under the Brief with the
Amendment 2 addition.

## Sources

* No PMC full text (Europe PMC: PMID 30220559, no PMCID). Elsevier open-archive files:
  * `article/1-s2.0-S1097276518306774-mmc2.pdf` = Document S2, article plus supplemental
    information (`https://ars.els-cdn.com/content/image/1-s2.0-S1097276518306774-mmc2.pdf`,
    SHA-256 cc1c2c51a0752bd78b006015450d2d8171ce20630bdea1d21cf495e9fa35c3e0); text
    extract `article/mmc2_text.txt`.
  * `article/1-s2.0-S1097276518306774-mmc1.pdf` (SHA-256
    feb5febac2238a791e02ad30f126fe2397b5ad9520fe5f3d7d3494468734d512).
* Deposit file list from the Mendeley public API: `article/deposit_metadata.json`.
* Deposit images already downloaded by `screen_files.py` are in `images/`.

## Criterion 1

Peer-reviewed article. Western blots are quantified: STAR Methods "Western Blotting
Quantification": "Signal intensities were normalized to the internal control (b-actin) on
the same blot before calculation of mean values." Met.

## Quantified western blot panels in figure order

Only one western blot is quantified by normalisation to a loading control with ≥ 2 groups:
**Fig. 2E** ("Quantification of NOP56 normalized to ACTB protein levels based on
quantitative western blots as in Figure S1D. The means from three biological replicates
(n = 3) are displayed"; siCTRL vs siNOP56 / siNOP58 / siFBL / siSMG6, Student's t tests).
The blots behind it are the LI-COR quantitative fluorescent westerns of Fig. S1D (STAR
Methods: "For quantitative fluorescent western blots (shown in Figure S1D and quantified in
Figure 2E) ... Protein levels were quantified using Image Studio (ver. 5.2)").

Western blots not quantified (shown only): Fig. 2B, 5D, 7C, S3F, S6B. All other
quantifications in the article are northern blots, RT-qPCR, RT-PCR gels, IF or flow
cytometry, which are outside the unit of analysis.

| Panel | Result | Evidence |
|---|---|---|
| Fig. 2E / S1D (NOP56 / ACTB; siCTRL vs siNOP56, siNOP58, siFBL, siSMG6) | **criterion 2 failed** | The deposited panel files are `S1D_NOP56.tif`, `S1D_NOP58.tif`, `S1D_FBL.tif`, `S1D_SMG6.tif`, `S1D_Beta_act.tif`. `blotverse inspect`: all five are 8-bit (RGB with identical channels, read as greyscale), effective bits 8, 224-226 grey levels, raw = no, reason "8-bit file". The deposit has no other image of this panel (the `F2E_NOP56_beta act*.pdf` files are PDF exports from Image Studio, not a native imager file or a 16-bit TIFF). |

For the record (not used for the decision): the only 16-bit raw files in the deposit
(`S1H_GAPDH.tif`, raw = yes; `S1H_NOP56_gen.tif`) are northern blots (Fig. S1H), and
`F_6B_*.tif` are upscaled 8-bit northern images. Criterion 7 under Amendment 2 was not
reached; the deposit holds Image Studio quantification PDFs `F2E_NOP56_beta act_Quant.pdf`
and `F2E_NOP56_beta act_2_Quant.pdf` (not opened).

## Decision

**Excluded.** First quantified panel in figure order: Fig. 2E (blots in Fig. S1D). First
failing criterion of the candidate: **criterion 2** (all deposited images of the panel are
8-bit). No panel qualifies, so no `fetch.sh` or `claim.yaml` was written.
