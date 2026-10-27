# Screening notes: 10.1016/j.molcel.2025.08.015

Boehm V, Wallmeroth D, Wulf PO, ... Landthaler M, Gehring NH. *Rapid UPF1 depletion
illuminates the temporal dynamics of the NMD-regulated human transcriptome.* Molecular Cell
85, 3524–3546 (18 Sep 2025). Peer-reviewed article (Cell Press Resource, CC BY 4.0); preprint
was bioRxiv 10.1101/2024.03.04.583328.

Deposit: Mendeley Data t9vzn5vwsz (v2, 10.17632/t9vzn5vwsz.2): `Boehm et al Figure1.zip`,
`Figure3.zip`, `FigureS1.zip`, `FigureS3.zip`, `Boehm et al Overview.pdf`.

## Sources saved in `article/`

| File | Source | SHA-256 |
|---|---|---|
| `boehm2025_molcel.pdf` | MDC edoc repository, publisher version: https://edoc.mdc-berlin.de/id/eprint/25694/1/25694.pdf | 1d7d760dc7f6e13976fb40c4d94dc8374f5fb4c2f5933022b4dfa92c8988f721 |
| `1-s2.0-S1097276525007002-mmc1.pdf` | Supplementary figures and legends (Document S1), taken from https://edoc.mdc-berlin.de/id/eprint/25694/2/25694suppl.zip | 2546088bc3d35cf0457a0638cb56d96c84a07121f94a5389c744a584f60826a1 |
| `Boehm et al Overview.pdf` | Mendeley deposit overview: https://data.mendeley.com/public-files/datasets/t9vzn5vwsz/files/7fc574d5-57d3-48c4-a2d8-4bc5951d9a4c/file_downloaded | 036bb7c0d8b3612361f5dcbd36d24a8f27bcc0f063793605fac1413dff782765 |

Cell.com, ScienceDirect and bioRxiv returned 403 to scripted requests. Europe PMC has no full
text for this article (PMID 40934927, no PMCID). I did not copy the supplementary tables
(mmc2–mmc4 xlsx, which hold the omics and oligo tables) into this folder.

## Criterion 1

The article contains western blots in Fig. 1C, 1F, 1G, 3G and Fig. S1C, S1L, S3H. Their
legends say what they are for:

* Fig. 1C: "(Top) Western blot of UPF1 protein after siRNA KD in HCT116 cells, and tubulin
  serves as a loading control; o.e., overexposure. (Middle) End-point quantitative
  reverse-transcription PCR (RT-qPCR) of SRSF2: relative levels of SRSF2 isoforms were
  quantified. (Bottom) qPCR of NMD targets ZFAS1 and GAS5, shown as log2 fold change
  (log2FC). Individual data points and mean (bars) are shown (n = 3)."
* Fig. 1F and 1G are "visualized as in C". Fig. 3G and Fig. S3H show a UPF1 western (top) with
  SRSF2 end-point RT-PCR and histone qPCR. Fig. S1C and S1L follow the layout of S1C.

In every one of these panels, the numbers and the n = 3 belong to the SRSF2 end-point RT-PCR
isoform ratio and the qPCR. The western blots are single representative images. No western
blot band is quantified or normalised to tubulin. Results: "Western blot confirmed UPF1
depletion by 2 h post-IAA treatment (Figure 1F)", which is qualitative. The Methods
(*Immunoblot analysis and antibodies*, *Quantification and statistical analysis*) describe no
densitometry. The deposit matches this. `Boehm et al Overview.pdf` shows one UPF1 membrane
(4–9 lanes, one replicate) and one tubulin membrane per panel. Only the SRSF2 RT-PCR agarose
gels hold three replicates ("Shown replicate").

The blot-named file found by the file screen, `Fig1C_GelDoc 2023-11-07 16hr 20min_SRSF2.scn`,
is the Bio-Rad GelDoc image of the SRSF2 end-point RT-PCR **agarose gel** (Overview p. 1; the
deposit description says "For RT-PCR images, TIF (.tif) and raw Bio-Rad Image Lab Image
Document (.scn) files are provided"). It is not a western blot. The western TIFs of Fig. 1C
(UPF1, UPF1_OE, UPF1_OE_high, Tubulin) are multi-page Fusion TIFs. `blotverse inspect`
rejects them as pseudo-coloured (criterion2.tsv), but that question is moot because no
western panel is quantified.

## Per-panel table

The only western blot panels quantified by the authors would be listed here. There are none,
so every western panel fails criterion 1.

| Panel | Criterion failed | Evidence |
|---|---|---|
| Fig. 1C (UPF1/tubulin WB, siRNA KD) | 1 | Western blot is representative only; quantification (n = 3) is of SRSF2 RT-PCR isoforms and qPCR; deposit has a single WB replicate |
| Fig. 1F, 1G, 3G | 1 | Same layout ("visualized as in C"); WB not quantified |
| Fig. S1C, S1L, S3H | 1 | Same layout; WB not quantified |

## Decision

**Excluded, criterion 1.** The article is peer-reviewed, but it has no western blot panel
that the authors quantify. All western blots (UPF1, V5, Myc, tubulin) are single
representative images. The quantified data (n = 3) are SRSF2 end-point RT-PCR isoform ratios
and qPCR. The `.scn` file is an RT-PCR agarose gel. No `fetch.sh` or `claim.yaml` was
written, because no panel qualifies.

## Rescreen under Amendment 2

First-frame rescreen (Amendment 2, point 5), 2026-10-27. Revised rules applied: criterion 7
no longer excludes; the total form of the target counts as a loading control for a
modification-specific target when each lane can be matched; normalisation to an
immunoprecipitated bait is still not a loading control.

Evidence reused unchanged: the legends of Fig. 1C, 1F, 1G, 3G, S1C, S1L and S3H, the Methods
and `Boehm et al Overview.pdf`. Every western blot (UPF1, V5, Myc, tubulin) is a single
representative image. The n = 3 quantifications belong to the SRSF2 end-point RT-PCR and
the qPCR. No western blot is phospho-specific, none is normalised to any reference band, and
the deposit holds one western replicate per panel.

Neither revised rule touches criterion 1 here: the article still has no western blot panel
quantified by the authors.

**Decision unchanged: excluded, first failing criterion 1.** No `fetch.sh` or
`claim.yaml` written.
