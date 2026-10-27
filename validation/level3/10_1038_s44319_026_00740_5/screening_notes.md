# Screening: 10.1038/s44319-026-00740-5

Kawamoto S, Horiguchi H, ..., Oike Y, Hara E. *Reevaluating the senolytic activity of a GLS1
inhibitor and an anti-PD-1 antibody: toward greater reproducibility and methodological
rigor.* EMBO Reports (2026). PMID 41933117, PMC13172396.
Deposit: EMBO SourceData `S-SCDT-10_1038-S44319-026-00740-5` (released 2026-03-13).

## Sources read (in `article/`)

* `PMC13172396.xml`: Europe PMC full-text XML (body, methods, legends).
* `legends.txt`: figure legends extracted from the XML.
* `44319_2026_740_Fig{1..7}_HTML.png`: publisher figure images (Fig1-5 = Figures 1-5,
  Fig6-7 = Figures EV1-EV2).
* `S-SCDT-10_1038-S44319-026-00740-5.json`: BioStudies study metadata (file list per panel).

## Western blot panels in figure order

The article has three western blot panels, all in Figure 1 (A: IMR-90, C: TIG-3, E: TIG-1;
p16INK4a, p21, LMNB1, beta-actin). None is quantified by the authors. Figures 2-5 and
EV1-EV2 contain no western blot (body weight, RT-qPCR, IHC, annexin V/TUNEL, cell counts).

| Panel | Result | Evidence |
|---|---|---|
| Fig. 1A (IMR-90: Control, DXR-Sen, Rep-Sen; p16, p21, LMNB1, beta-actin) | criterion 1 fails (not quantified) | One lane per condition (lanes 1-3), no densitometry plot. Legend: "subjected to western blot analysis ... to confirm their senescent state. beta-actin was used as a loading control (A, C, E). Quantification of EdU-positive cells is shown in the right panels (A, C)." The only quantification and statistics (one-way ANOVA, Dunnett) in 1A are for EdU. Statistical-analysis section lists Fig. 1A,C tests only for the EdU data. WB methods give no densitometry. |
| Fig. 1C (TIG-3: same design) | criterion 1 fails (not quantified) | Same as 1A: single lane per condition, only EdU quantified. |
| Fig. 1E (TIG-1: Control, ETO-Sen; p16, LMNB1, beta-actin) | criterion 1 fails (not quantified) | Two lanes, no quantification at all in panel E. |

For information only (not needed for the decision): `blotverse inspect` on
`images/1A_WB_LMNB1.tif` gives 16-bit, raw = yes (2531 saturated px). The deposit's raw WB
TIFFs (1A, 1C, 1E) are therefore readable, but the panels they belong to are illustrative,
not quantified.

## Decision

**Excluded. Criterion 1** (peer-reviewed article, but no western blot panel quantified by
the authors). The first western blot panel in figure order, Fig. 1A, shows one lane per
condition to confirm senescence and has no band quantification; the same holds for 1C and 1E.

No `fetch.sh` or `claim.yaml` was prepared (no qualifying panel). The four files already in
`images/` (from `screen_files.py`) were left untouched.
