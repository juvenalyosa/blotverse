# Screening: 10.1038/s41467-026-71753-w

Volk et al. (2026) "Early life exposure to N-nitrosamine drives genotoxicity, mutagenesis,
and tumorigenesis in DNA repair-deficient mice", *Nature Communications* 17:5200
(peer-reviewed, research article; PMID 41980981, PMC13254281).
Deposit: Zenodo 10.5281/zenodo.19122404 (`Western_Blot.zip` = LI-COR Image Studio projects
with raw Odyssey CLx TIFFs; `Source_Data.zip` = per-figure source data).

Sources read: publisher HTML and Europe PMC full-text XML (`article/`), all figure legends,
Supplementary Information PDF (Supplementary Fig. 11 = labelled cropped blots of every
quantified western, rendered as `article/SupplementaryFig11_overview.png`; the PDF itself is
in `authors/` because its other supplementary figures print per-replicate points and
p-values), the Image Studio project acquisition names (to tie each raw image to its blot).

## Quantified western blot panels in figure order

Main figures: Fig. 1e (γH2AX, p-p53, p21, RAD51), Fig. 1g (CYP2E1), Fig. 2a (pSTAT1,
pSTAT3, CD3e, HO-1), Fig. 2c (cleaved caspase-3), Fig. 6b (p21 by sex), Fig. 7b (γH2AX, WT);
then Supplementary Figs 1c, 2a-c. All are normalised to Total Protein Stain and compare
≥ 2 groups. Examination stopped at the first panel, which qualifies.

| Panel | Result | Evidence |
|---|---|---|
| Fig. 1e, γH2AX (14+0d) | **included** | see below |

Fig. 1e γH2AX, criterion by criterion:

1. Peer-reviewed article (Nat Commun 2026), western blots quantified (fold change vs
   untreated juveniles, two-way ANOVA). Met.
2. Raw files. Image Studio acquisition "Blot 66 - STAT1, PCNA, pH2AX" = `0000817_02`
   (`0000817_03` is a byte-identical duplicate) and "Blot 66 - TPS" = `0000807_01`.
   `blotverse inspect`: `0000817_02_800.TIF`, `0000817_02_700.TIF`, `0000807_01_700.TIF`
   all 16-bit (LI-COR floating-point data), raw = yes, no rejection (warning: floating-point
   image rescaled to 16-bit; detector saturation cannot be checked from pixel values). Met.
3. Uncropped. The images are full Odyssey scans of the whole membrane (cut into three
   strips for STAT1 / PCNA / γH2AX), with all 24 sample lanes and the ladders; the TPS image
   shows the whole membrane with the same 24 lanes. Met.
4. n = 6 mice/group (3 males, 3 females) per the legend; 4 groups x 6 = 24 lanes, all on
   Blot 66, present in both the γH2AX strip and the TPS image. Met.
5. Lane mapping from Supplementary Fig. 11b (labelled crop of the same blot): left to right
   Juvenile NT (M1 M2 M3 F1 F2 F3) | Juvenile NDMA (M1-F3) | Adult NT (M1-F3) | Adult NDMA
   (M1-F3), each block of six bounded by protein-ladder lanes, which are visible in the raw
   image (ladders at both ends and between blocks, 700 nm channel). The raw membrane has one
   additional sample lane at the far left, between the first two ladders; it is outside the
   region shown in Supplementary Fig. 11b and belongs to none of the four groups, so it is
   left out. Mapping unambiguous without the authors' numbers. Met.
6. Loading control: Total Protein Stain image of the same membrane ("Blot 66 - TPS",
   `0000807_01_700.TIF`), all 24 lanes visible. Met.
7. Per-replicate values exist: `Source_Data_Fig1_R2.xlsx`, sheet `Fig1e_PH2AX` (per-lane
   Image Studio signals, /TPS and fold-change values per mouse; 6 per group), plus the Image
   Studio export `WS_D0_RGAM_Blot66_pH2AX.xls`. Opened only to confirm values exist and n.
   Met.
8. Lanes vertical in both images (no rotation needed). Met.

Note for annotators (not an inclusion issue): the γH2AX strip (bottom strip, 800 nm) has a
blotchy background; the specific signal is strongest in the Juvenile NDMA block.

## Primary contrast and claim

Juvenile NDMA vs Juvenile NT, up, p < 0.0001 (two-way ANOVA, Šídák), n = 6 per group:
"Western blot analysis reinforced these age-specific disparities, revealing significantly
elevated γH2AX only in younger animals after NDMA treatment (Fig. 1e)." (Results). Details
in `claim.yaml`.

## Files

`fetch.sh` re-downloads everything (SHA-256 checked; publisher HTML/XML hashes recorded for
reference only). `images/`: `0000817_02_800.TIF` (target), `0000817_02_700.TIF` (ladders /
PCNA, orientation), `0000807_01_700.TIF` (TPS). `authors/`: Source data Fig. 1, Image Studio
database (contains per-shape signals) and exports for Blot 66, results workbook, Fig. 1 image,
SI PDF. The image file pre-downloaded by the file screen
(`IP_JUV_vs_ADULT/.../0000051_01_W.TIF`, intraperitoneal experiment, Supplementary Fig. 1c)
and the unrelated deposit archives were removed.

## Decision

**Included.** Experiment: Fig. 1e, γH2AX normalised to Total Protein Stain, Aag-/-;Mgmt-/-
liver at 14+0d, 4 groups x 6 mice (Blot 66).
