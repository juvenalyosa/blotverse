# Screening notes: 10.1016/j.molcel.2025.10.026

Gracia, Zhang et al., "HSP90 buffers deleterious genetic variations in BRCA1", *Mol Cell*
85(23):4365-4378.e10 (2025). Peer-reviewed (PMC12648472, author manuscript). Deposit:
Mendeley Data th44573vw6 v1 (doi:10.17632/th44573vw6.1).

Article files in `article/`: PMC HTML (`pmc.html`, text in `pmc.txt`), figures
(`fig1.jpg`-`fig5.jpg`: the five PMC figure images, graphical abstract plus Figures 1-4), supplemental figures and legends (`mmc1_supplement.pdf`,
from `https://ars.els-cdn.com/content/image/1-s2.0-S1097276525008676-mmc1.pdf`).

Criterion 1: met. Methods: "Quantification of immunoblots was performed using ImageJ.
Results for FLAG levels were normalized over actin which served as the loading control in
each experiment."

## Quantified western blot panels, in figure order

Not quantified (no normalised band comparison): 1C, 2C (co-IP), S1B-S1E, S1K, S2C-S2E.

| Panel | Result | Evidence |
|---|---|---|
| Fig. 3E-F (CHX chase, 3xF-BRCA1 WT / A1708V / L1705R under 37 °C/DMSO, 37 °C/HSP90inh, 39 °C/DMSO, 39 °C/HSP90inh; FLAG normalised to actin; 3F quantification, 3G AUC comparison vs WT; 3-6 independent experiments) | **fails criterion 2** | `blotverse inspect` on all eight deposited files `3E_WB_{37oC,39oC}_{DMSO,HSP90i}_{FLAG,Actin}.tif`: 8 bits, raw = no, reason "8-bit file" (several also contain 0.16-1.0 M saturated pixels). |
| Fig. S2F (basal BRCA1 levels of 15 mutant lines; quantification, one-way ANOVA with Dunnett vs WT) | fails criterion 2 | `S2F_WB_FLAG.tif` and `S2F_WB_Actin.tif`: 8 bits, raw = no, "8-bit file". |
| Fig. S3H (CHX chase with 20 nM STA-9090, WT / A1708V / S1715R at 0, 1, 2, 4, 6 h; FLAG with GAPDH loading control; Log2 FC plot) | fails criterion 4 | Criterion 2 met: `S3H_WB_FLAG.tif` and `S3H_WB_GAPDH.tif` are 16-bit, raw = yes. Criterion 3 met (whole membrane, 15 lanes). But the deposit holds one membrane (one lane per variant and time point), and the source table `S3H.xlsx` has a single row of 15 values, i.e. n = 1 per group (S3 legend: "at least two independent experiments"). Fewer than 3 replicates per group. |
| Fig. S3I (steady-state BRCA1 variants ± 12.5 / 15 nM NVP-HSP990; FLAG / actin; Log2 FC vs DMSO) | fails criterion 2 | `S3I_WB_FLAG.tif` and `S3I_WB_Actin.tif`: 8 bits, raw = no, "8-bit file" (also n = 2 per group in `S3I.xlsx`). |

No panel meets criteria 2-8.

## Decision

**Excluded.** First quantified panel in figure order: Fig. 3E-F, which fails
**criterion 2** (deposited blot images are 8-bit TIFF exports). The only 16-bit blot pair
in the deposit (S3H, found by the file screen) fails criterion 4 (n = 1 per group).

No `fetch.sh` or `claim.yaml` written (no qualifying panel). `images/` holds the images
downloaded by `screen_files.py`; S3H_WB_GAPDH.tif, S3I images and the source tables were
inspected from a scratch copy only and are not stored here.
