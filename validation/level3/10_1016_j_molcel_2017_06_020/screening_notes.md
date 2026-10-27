# Screening: 10.1016/j.molcel.2017.06.020

Second sampling frame, rank 133. Screened under the brief with the Amendment 2 addition
(criterion 7 records only; phospho/total accepted as loading control; co-IP bait is not).

Morchikh, Cribier et al. (2017) "HEXIM1 and NEAT1 Long Non-coding RNA Form a Multi-subunit
Complex that Regulates DNA-Mediated Innate Immune Response", *Molecular Cell* 67:387-399
(peer-reviewed research article; PMID 28712728; not in PMC or Europe PMC full text). Deposit:
Mendeley Data 968jfbz8xd v1 (doi:10.17632/968jfbz8xd.1), "Unprocessed imaging data (Western
Blot and microscopy)": 12 zip archives, one per figure (Figures 1-6, S1-S6). The frame-2
screen extracted Figure 1.zip (`1A_silver staining.tif` was rejected as pseudo-colour,
`1C_CDK9 WB.tif` raw = yes).

Sources read (in `article/`): Elsevier "Document S2. Article plus Supplemental Information"
(`article_plus_SI_mmc5.pdf`: full text, main legends, STAR Methods, supplementary legends),
Document S1 (`DocumentS1_mmc1.pdf`), main figures `gr1.jpg`-`gr7.jpg` (ars.els-cdn.com,
high resolution), Mendeley API metadata (`deposit_metadata.json`). The cell.com and
ScienceDirect HTML pages refuse scripted access (HTTP 403). Open-access lookup APIs that require an e-mail (Unpaywall, Crossref/OpenAlex "mailto")
were not used, and no personal identifier was sent to any service. The PDFs came from the
publisher's static file server without them.

STAR Methods, "Quantification and statistical analysis": "Data in Figures 4 and 6 and S4 are
represented as the mean ± standard deviation of the mean (SD) ... Data in Figures S1, S3, and
S6 show one representative experiment." Each of those quantifications is qRT-PCR (IFNα/β, MXA,
NEAT1, 7SK), RNA-seq or microscopy spot counting. No western blot is quantified anywhere.

## Western blot panels, in figure order

| Panel | Result | Evidence |
|---|---|---|
| Fig. 1B, 1C | not quantified | FLAG-IP and endogenous IPs analysed by WB (silver stain 1A). No densitometry. |
| Fig. 2A, 2B | not quantified | Glycerol-gradient fractions and CycT1 IP / FLAG-IP / re-IP immunoblots; the only graph (2A right) is 7SK qRT-PCR per fraction. |
| Fig. 3D | not quantified | FLAG-IP ± RNase / siRNA immunoblot; 3B and 3C are RNA qRT-PCR, 3E microscopy. |
| Fig. 4A, 4B (lower panels) | not quantified | pIRF3, IRF3, pTBK1, TBK1 immunoblots of siRNA-treated HeLa cells. The bar charts above them are qRT-PCR fold induction (mean ± SD, n = 3 and n = 2). The blots are shown without densitometry or normalisation. |
| Fig. 5A, 5B | not quantified | Input / FLAG-IP / re-IP and cGAS IP immunoblots (actin in inputs), no quantification. |
| Fig. 6A, 6C | not quantified | Knockdown checks (STING, HEXIM1, Ku70 / actin) shown as blots only; 6A NEAT1 and 6B IFNβ are qRT-PCR. 6C is IP. |
| Fig. S1-S6 | not quantified | S1, S2, S3C, S5: IP immunoblots; S4 left: knockdown blots, right: NEAT1 qRT-PCR; S6: siRNA blots + IFNβ qRT-PCR (n = 1). |

## Decision

**Excluded, criterion 1**: no western blot panel is quantified by the authors. Every
quantification is qRT-PCR, sequencing or imaging, and the blots are shown as qualitative
evidence. No panel is an experiment under *Unit of analysis*, so criteria 2-8 were not
examined, and no `fetch.sh` or `claim.yaml` was prepared. Criterion 7 (records only): no
per-replicate source data are deposited or in the supplement.
