# Screening notes: 10.1016/j.celrep.2018.03.054

Feltham et al., "Mind Bomb Regulates Cell Death during TNF Signaling by Suppressing RIPK1's
Cytotoxic Potential", *Cell Reports* 23:470-484 (2018). PMID 29642005, PMC5912950.
Second frame, rank 13. Deposit: Mendeley Data 52t6f2m8k5 v1 (13 zips, one per figure:
Figures 1-6 and Supplementary Figures 1-7). Screened 2026-10-27 under the Brief with the
Amendment 2 addition.

**Decision: excluded. First failing criterion: 2 (Fig. 6G).**

## Sources (in `article/`)

* `PMC5912950.xml`: Europe PMC full-text XML (legends, STAR Methods),
  `https://www.ebi.ac.uk/europepmc/webservices/rest/PMC5912950/fullTextXML`, SHA-256
  74119cebfba91339aa17725606a46188dd5cd538d2881ef614472c4568dcc2ea.
* `1-s2.0-S2211124718303991-mmc1.pdf` (Document S1, Supplemental Experimental Procedures and
  Figures S1-S7; SHA-256 473492a51d28bfc806c6531e64e8eb407aaf07fcaf7820efc6237b4d999a89a8)
  and `1-s2.0-S2211124718303991-mmc2.pdf` (Document S2, article plus supplement; SHA-256
  6f825fdad8cab501f83f9322f279a39d052dcd3d371a818402907c94c2591240), from
  `https://ars.els-cdn.com/content/image/1-s2.0-S2211124718303991-mmc{1,2}.pdf`. Text extracts
  `mmc1_text.txt`, `mmc2_text.txt` (macOS PDFKit).
* `1-s2.0-S2211124718303991-gr{2,5,6}_lrg.jpg`: Figures 2, 5 and 6 (SHA-256 29f87970...,
  e70d3c9b..., 528f0432...).
* `deposit_metadata.json`: Mendeley public API record of the deposit (file list, SHA-256).
* `archives/`: deposit zips for Supplementary Figure 1 (downloaded by `screen_files.py`) and
  Figures 2, 5 and 6 (downloaded for this screen; SHA-256 1c5825e7..., c7a58505...,
  475820415b..., matching the deposit metadata).

## Criterion 1

Peer-reviewed article, not a preprint. Western blots are quantified in Figs. 2D, 5A, 5E, 6F
and 6G (legends: "Quantification of RIPK1 bound to caspase-8 is shown", "Quantification for
ubiquitylation is shown at 15 min time point", etc.). No supplementary figure has a
quantified blot (S1-S7 legends checked). The methods give no densitometry procedure. Met.

## Quantified western blot panels, in figure order

| Panel | Quantity plotted | Fits the unit of analysis? |
|---|---|---|
| Fig. 2D | "RIPK1 bound to Casp-8", y axis "Absolute Intensity (Fold change)", caspase-8 IP, siCTRL vs siMIB2 at 8 h and 24 h, one bar per condition | No: not normalised to a loading control (absolute intensity of a co-IP band) |
| Fig. 5A | "Total RIPK1 Ub relative to TNFR1 (15 min)", FLAG-TNF (complex-I) pulldown, Ctrl vs MIB1/2 DKO | No: normalised to TNFR1 in the immunoprecipitate (co-IP bait), still not a loading control under Amendment 2 |
| Fig. 5E | "Ub-smear above line", y axis "Absolute Intensity (Fold change)", TUBE pulldown, ± MIB2WT at 15' and 30' | No: absolute intensity, no loading control |
| Fig. 6F | "Total RIPK1 Ub relative to TNFR1", complex-I pulldown, RIPK1WT vs RIPK1K377R | No: normalised to TNFR1 in the immunoprecipitate (co-IP bait) |
| Fig. 6G | "P-S166-RIPK1 relative to total RIPK1", RIPK1 IP, RIPK1WT vs RIPK1K377R at 0/30/60 min | Yes under Amendment 2 point 3 (phospho-specific target normalised to the total form of the same protein, same lanes) |

Note on Fig. 6G: total RIPK1 is also the immunoprecipitated protein. It was treated as a
phospho/total ratio of the target (Amendment 2 point 3), not as normalisation of a
co-precipitated partner to a bait. If it were read as bait normalisation, no panel would fit
the unit of analysis and the candidate would fail criterion 1 instead; either way it is
excluded.

## Panels examined

| Panel | Criterion failed | Evidence |
|---|---|---|
| Fig. 6G | 2 | Deposit `Feltham et al. Figure 6.zip`: `6G_pS166 WB.tiff`, `6G_RIPK1 WB.tiff`, `6G_RIPK1-input WB.tiff`, `6G_ACTIN WB.tiff`. `blotverse inspect`: all four 8-bit (raw = no, "8-bit file"; RGB file with identical channels). Also a single experiment with no error bars (would fail criterion 4). |

For the record, the other quantified panels (not experiments under the protocol) would also
fail criterion 2: Fig. 2D files (`2D_RIPK1/CASP8/MIB2/ACTIN WB.tiff`) are "not a readable TIFF
file"; Fig. 5A files are 8-bit; Fig. 5E files are "not a readable TIFF file"; Fig. 6F files are
8-bit or pseudo-coloured (`6F_RIPK1 WB.tiff`). Of the deposit files checked, only some
Supplementary Figure 1 TIFFs are raw (e.g. `S1F_MIB2 WB.tiff`), and S1 is not quantified.

Criterion 7 (recorded, no longer excluding): no source-data table or per-replicate numbers
were found (no xlsx/csv in the article supplement or the deposit).

## Decision

Excluded. First quantified panel that fits the unit of analysis: Fig. 6G; first failing
criterion: 2 (8-bit exports only).
