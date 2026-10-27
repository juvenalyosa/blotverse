# Screening notes: 10.1038/s44318-026-00706-2

Article: Fu, Wang et al., "Regulation of DMSP organosulfur cycling in ubiquitous Roseobacter
marine bacteria", The EMBO Journal (2026), PMID 41663842, PMC12992908. It is peer-reviewed
(published EMBO J article, not a preprint).
Deposit: EMBO SourceData S-SCDT-10_1038-S44318-026-00706-2 (released 2026-02-04).

Files in `article/`:

| file | source | SHA-256 |
|---|---|---|
| `PMC12992908.xml` | https://www.ebi.ac.uk/europepmc/webservices/rest/PMC12992908/fullTextXML | 945b0ca4c3fbb24e2b578a595a588dbcf864186085800b7e107d84a8f3e3dd9f |
| `44318_2026_706_Fig2_HTML.png` | https://media.springernature.com/full/springer-static/image/art%3A10.1038%2Fs44318-026-00706-2/MediaObjects/44318_2026_706_Fig2_HTML.png | 9fc6f05923ce7b1c4ce35a9b029eb7e865dbeefa06c52e5e576fa1879ba3a291 |
| `44318_2026_706_MOESM1_ESM.pdf` (Appendix) | https://static-content.springer.com/esm/art%3A10.1038%2Fs44318-026-00706-2/MediaObjects/44318_2026_706_MOESM1_ESM.pdf | 17c0e749bacfa7b183ed6580442cd217489cf6bfc50fdace9cce88a60704d8c5 |

`images/Western_blot_treated_with_DMSP.tif` (SHA-256
0b1b0db0b60a3e85d696d562c08f51942f8302592a7e18e9bcb28565f4f9fe54) was downloaded by
`screen_files.py`; it is the raw image of Fig. 2D.

## Western blot panels in the article

Every western blot in the article and Appendix was searched for (full-text XML, figure
legends, Appendix text). There are three, all of DmdA:

| panel | quantified by authors? | failing criterion | evidence |
|---|---|---|---|
| Fig. 2D (DmdA, DSS-3 vs ΔdmdR, CM / 30 µM / 6 mM DMSP) | no | 1 | Figure shows the blot only, one lane per condition, no bar chart, no normalisation to a loading control. Results: "Western blotting with DmdA antibody indicated that DmdA protein levels increased with DMSP and acrylate concentration in wild-type R. pomeroyi DSS-3, but were constitutively high in ΔdmdR cells (Fig. 2D,E; Appendix Fig. S2h)". The "mean ± SD (n ≥ 3)" and t-test in the Fig. 2 legend refer to the RT-qPCR panel C. The SourceData deposit has only the .tif for 2D, no xlsx. (For the record only: `blotverse inspect` gives 16-bit, raw = yes.) |
| Fig. 2E (DmdA, DSS-3 vs ΔdmdR, CM / 30 µM / 3 mM acrylate) | no | 1 | Same as 2D: blot only, no quantification, no source-data table (deposit has only `Western_blot_treated_with_acrylate.tif`). |
| Appendix Fig. S2h (DmdA in DSS-3, ΔdmdA, ΔdmdA/dmdA, 6 mM DMSP) | no | 1 | Legend: "Coomassie brilliant blue stained SDS-PAGE gel to show the equal amounts of loaded protein ... Western blot analysis of DmdA". Loading shown qualitatively, no densitometry. |

Methods ("Western blot assay") describe ECL detection and a Coomassie-stained gel "to
visualize the equal amount of loaded protein"; no densitometry, ImageJ or normalisation is
described. Fig. 6G is an in-gel peroxidase activity stain, not a western blot.

## Decision

**Excluded, criterion 1**: peer-reviewed article, but no western blot panel is quantified
by the authors (Fig. 2D/E and Appendix Fig. S2h are qualitative blots only). No panel was
examined against criteria 2–8, so no `fetch.sh` or `claim.yaml` was prepared.

## Rescreen under Amendment 2

First-frame rescreen (Amendment 2, point 5), 2026-10-27. The revised rules (criterion 7 no
longer excludes; phospho / total accepted as loading control when each lane can be matched;
co-IP bait normalisation still not) do not touch criterion 1. The full text, figure legends
and Appendix were re-searched for phospho-specific or other modification-specific blots: none
(the only western blots are DmdA in Fig. 2D, 2E and Appendix Fig. S2h; no phospho antibody,
no co-IP). None of the three panels is quantified (no densitometry, no normalisation, no bar
chart), so there is still no panel to examine against criteria 2–8.

| Panel | Revised outcome | Evidence |
|---|---|---|
| Fig. 2D | 1 (unchanged) | Blot only, one lane per condition, no quantification (see above). |
| Fig. 2E | 1 (unchanged) | Same as 2D. |
| Appendix Fig. S2h | 1 (unchanged) | Coomassie loading shown qualitatively, no densitometry. |

### Decision (rescreen)

**Excluded, criterion 1** (unchanged): no western blot panel quantified by the authors.
No `fetch.sh` or `claim.yaml` prepared.
