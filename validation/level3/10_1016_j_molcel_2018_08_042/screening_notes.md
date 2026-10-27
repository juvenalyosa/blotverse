# Screening notes: 10.1016/j.molcel.2018.08.042

Delgado-Benito, Rosen et al., "The Chromatin Reader ZMYND8 Regulates Igh Enhancers to Promote
Immunoglobulin Class Switch Recombination", *Molecular Cell* 72:636-649 (2018). PMID 30293785,
PMC6242708. Second frame, rank 15. Deposit: Mendeley Data jktyjvy9hg v1 (zips for Figures 1,
S1, S2, S3). Screened 2026-10-27 under the Brief with the Amendment 2 addition.

**Decision: excluded. First failing criterion: 1 (no western blot panel quantified).**

## Sources (in `article/`)

* `PMC6242708.xml`: Europe PMC full-text XML,
  `https://www.ebi.ac.uk/europepmc/webservices/rest/PMC6242708/fullTextXML`, SHA-256
  c4e3c6ca0b519553858271fb6d7388ef28298c52817d27dd87e30b704206bd6e.
* `1-s2.0-S1097276518307020-mmc1.pdf` (Document S1, Figures S1-S7 and Tables S1 and S5;
  SHA-256 10d875067be205f84496a614244a70f771b3d4c47f12214934cd30db4ab2fb2e) and
  `1-s2.0-S1097276518307020-mmc5.pdf` (Document S2, article plus supplement; SHA-256
  af7194eb6be711a800e576c6b96e9ae74c94c482724213cb0ed081069cc8f871), from
  `https://ars.els-cdn.com/content/image/1-s2.0-S1097276518307020-mmc{1,5}.pdf`; text extracts
  `mmc1_text.txt`, `mmc5_text.txt`.
* `1-s2.0-S1097276518307020-gr1.jpg`: Figure 1 (SHA-256 6428d3b7...).
* `deposit_metadata.json`: Mendeley public API record of the deposit.

## Criterion 1

Peer-reviewed article, not a preprint. Every western blot in the article is shown as a
representative image without quantification:

| Panel | Western blot content | Quantified? |
|---|---|---|
| Fig. 1B (left bottom) | ZMYND8 and β-actin, Ctrl / gAicda / gZmynd8-1 / gZmynd8-2, threefold dilutions | No; the dot plot of Fig. 1B is CSR (% IgA+) by flow cytometry |
| Fig. 1D (right) | "Representative WB analysis" of WT, Zmynd8-KO1, KO2 CH12 lines | No; the bar graph is CSR by flow cytometry |
| Fig. S1A | Rif1 WT vs Rif1FH/FH splenocytes, threefold dilutions | No |
| Fig. S1H-I | anti-Flag(Rif1) and anti-ZMYND8 co-IPs, "representative" | No |
| Fig. S2A, S2D | whole-cell extracts, Cd19Cre/+ vs Zmynd8F/FCd19Cre/+; time course | No |
| Fig. S3B | WB of Zmynd8-/- iMEF clones | No |

The Methods describe western blotting ("performed on whole cell lysates prepared by lysis in
RIPA buffer") but no densitometry, ImageJ or normalisation. Every quantified graph is flow
cytometry, qPCR, ChIP-seq, RNA-seq, GRO-seq or sequencing. Criterion 1 fails: no western blot
panel quantified by the authors.

## Panels examined

None (no quantified western blot panel). For the record, the deposit's Figure 1 zip has
`1B_Actin WB.tif` (raw = yes in `criterion2_frame2.tsv`), `1B_ZMYND8 WB.tif`,
`1D_Actin WB.tif` and `1D_ZMYND8 WB.tif`; they were not assessed further.

Criterion 7 (recorded): no western blot numbers exist (Tables S2-S4 are gene lists).

## Decision

Excluded, criterion 1: the western blots are representative only; the article quantifies no
western blot panel.
