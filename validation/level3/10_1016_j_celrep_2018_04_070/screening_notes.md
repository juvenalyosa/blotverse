# Screening notes: 10.1016/j.celrep.2018.04.070

Souquet et al., "Nup133 Is Required for Proper Nuclear Pore Basket Assembly and Dynamics in
Embryonic Stem Cells", Cell Reports 23:2443–2454 (2018), PMC5995580. Deposit: Mendeley Data
w2bwx2528j v1, a single zip, `Souquet et al. figure 4D_Raw data.zip`. Its description reads: "original
16-bit images and montages of the western blots used for Fig. 3D [sic; the article cites it for
Fig. 4D] … i: left panels; ii: right panels … In (ii), 3 independently processed samples".
Second frame, rank 69. Screened under the brief with the Amendment 2 addition.

The article came from PMC (`article/pmc.html`). The Supplemental Information came from the
Elsevier CDN (PII S2211124718306296): mmc1 is the Supplemental Information and mmc2 is the
article plus SI. No API that needs an email address was used.

## Criterion 1: no western blot panel quantified by the authors

The article is peer reviewed (Cell Reports), and it has western blots, but none is quantified:

| Panel | What it is | Quantified? |
|---|---|---|
| Fig. 4D (the deposited blots) | pull-down with Nup153-NTD baits, WT vs Nup133−/− extracts; inputs and eluates (64×, 48×) probed for Y-complex Nups and Tpr. Nup98 and GAPDH are "negative controls" (non-binding proteins), not loading controls | **no**. The comparison of WT and KO eluates is quantified by **mass spectrometry** (Fig. 4E, volcano plot, MaxLFQ). The blots are shown only as images |
| Supp. Fig. S1D | Nup133, Tpr and Nup98 levels in WT vs Nup133−/−, γ-tubulin as loading control | no |
| Supp. Fig. S3A, S3F (bottom) | expression of the GFP/mCherry-Nup133 constructs, γ-tubulin or GAPDH as loading control | no |
| Figs. 1–3, 5, S1–S3 (other panels) | 3D-SIM counting, NE fluorescence intensities, iFRAP | yes, but not western blots |

The Results describe Fig. 4D qualitatively ("Western blot and quantitative mass spectrometry
analyses detected Nup133 only in WT samples … a similar enrichment of Tpr and Y-complex proteins
… (Figures 4D and 4E)"). The figure has no densitometry, normalisation or statistics for any
blot (checked on the rendered Fig. 4 page). Fig. 4D would also not be a unit of analysis: it is
a pull-down, and GAPDH there is a negative-binding control.

**Decision: excluded, criterion 1.** Criteria 2–8 were not examined. The criterion-2 screen
found a 16-bit TIFF (`His-Nup153 pull-down (nitro i) GAPDH/20170206_1559-1.tif`), but it
belongs to the unquantified Fig. 4D. Amendment 2 changes nothing here.

## Files

* `article/`: `pmc.html`, `S2211124718306296-mmc1.pdf`, `S2211124718306296-mmc2.pdf`. They
  contain no western blot values.
* `images/` and `archives/` were left as downloaded by `screen_files.py`. No `fetch.sh` or
  `claim.yaml` was written.
* No file with the authors' numbers for a western blot exists, and none was downloaded.
