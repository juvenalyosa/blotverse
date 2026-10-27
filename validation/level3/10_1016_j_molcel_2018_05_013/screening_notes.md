# Screening notes: 10.1016/j.molcel.2018.05.013

Dionne et al., "Direct Phosphorylation of SRC Homology 3 Domains by Tyrosine Kinase Receptors
Disassembles Ligand-Induced Signaling Networks", Mol Cell 70 (2018). PMID 29910111,
PMC6014926. Peer-reviewed journal article, not a preprint. Deposit: Mendeley Data
fyt8893h8z v1 ("Western Blots and Immunofluorescence datasets"). Second sampling frame
(rank 92), screened 2026-10-27 under the brief with the Amendment 2 addition.

## Sources

* `article/PMC6014926.xml`: PMC full text (NCBI efetch; Europe PMC full-text endpoint
  returned HTTP 500). SHA-256 94af027209803a81a9a3d0c3862ebb379ef064c3d3edcfab364564a57be9f303.
* `article/mmc1_supplementary.pdf`: publisher supplement (Figures S1–S6, Tables S1–S3),
  `https://ars.els-cdn.com/content/image/1-s2.0-S1097276518303873-mmc1.pdf`,
  SHA-256 b3642937ab398c309766ff4bee856281453b77712cd5f591d69435fd989cd9d6.
* `article/gr3_lrg.jpg`: Figure 3 (publisher, `...-gr3_lrg.jpg`), SHA-256 c2f155cade9959c563dc6086ea9ed83b8c06f42f8c9be5914ce55ec4466a931a
  (examined for the Fig. 3E quantification; it prints plotted means, but no panel was
  included, so no `authors/` split was made).
* Deposit file list from the Mendeley public API. The deposit has 14 zips: Figures 2A, 2B,
  4A-D, 5, 6A, 6E, S1A, S2C, S2E, S3B, S4, S5, S6A, S6B. Contents (listed during
  screening; S1A–S6B zips downloaded to a scratch folder only):
  * 2A: WCL and IP WB (FLAG, pSH3, EphA4); 2B, 6A, S6A, S6B: autoradiograms and Coomassie
    (in vitro kinase assays / peptide arrays); 6E: WCL and IP WB CBL / FLAG; S1A: WB
    actin / FLAG / NCK; S2C: Coomassie; S2E: Ponceau / pYSH3; S3B: IP and WCL WB
    (BCAR1, CHN1, FLAG, PAK); S5: PNG exports (Dock, FLAG, GAPDH);
  * 4A-D, 5, S4: immunofluorescence microscopy only (S4 = panels S4A–S4L, no S4N blot).
  * **No file for Figure 3 (3E) and none for Figure S4N.**
  * No xlsx/csv with western blot values in the deposit. The article's supplemental
    datasets 1–4 (xlsx) hold SH3 lists, SWATH/SRM and peptide-array data, not blot
    densitometry.

## Quantified western blots in the article (figure order)

Methods: "Quantification was performed with the Amersham Imager 600 analysis software";
"For Western blot quantification ... ANOVA followed by Fisher LSD". No loading-control
normalisation is described anywhere.

* Western blot panels shown only as representative images, not quantified: Fig. 2A
  ("A representative image of 3 experiments"), 6E (same), S1A, S2E, S3B ("A
  representative image of 3 experiments"), S5 ("representative image of 2 experiments").
  Fig. 2B, 6A, S6A/B are autoradiography of in vitro kinase assays / peptide arrays, not
  western blots (Fig. 2E kinetics are 32P incorporation).
* **Fig. 3E** (first quantified western blot): time course after ephrin-B2 stimulation;
  "Phosphorylation dynamics of EPHA4 (anti-pTyr Western blotting)" plotted as
  "Phosphorylation or interaction level (relative to maximum)", * p < 0.05 (ANOVA),
  together with SRM data. Neither the legend nor the methods say that the pTyr signal is
  normalised to a housekeeping protein, total protein or total EPHA4; the stated scaling is
  to the time-course maximum. No blot image is shown in the figure.
* **Fig. S4N**: "Quantification of Western blot analyses of FLAG immuno-precipitates ...
  The amount of PAK1 (abundance ratio) was quantified in NCK2 IPs ... normalized to the WT
  EPHA4 + WT NCK2 condition. A total of 4 experiments", * p < 0.05. This is a co-IP
  readout (PAK1 recovered with the FLAG-NCK2 bait); under Amendment 2 bait normalisation is
  still not a loading control.

## Panels examined

| Panel | First failing criterion | Evidence |
|---|---|---|
| Fig. 3E (EPHA4 anti-pTyr, 0–360 min ephrin-B2) | 1 (not a unit of analysis); would fail 2 | Quantified, but normalisation is "relative to maximum"; no loading control (housekeeping, total protein, or total EPHA4 as phospho/total) is stated in legend or methods, so the panel does not meet *Unit of analysis* even with Amendment 2 point 3. Even if it did, it would fail criterion 2: the deposit contains no image for Figure 3. C7 (recorded only): no per-replicate numbers in the article, supplement or deposit. |
| Fig. S4N (PAK1 in FLAG-NCK2 IPs, WT/KD EPHA4 x NCK2 WT/3W/K/3Y/F) | 1 (not a unit of analysis); would fail 2 | Co-IP abundance ratio of PAK1 in NCK2 IPs: bait normalisation, which Amendment 2 keeps out of the loading-control definition. No S4N blot in the deposit (S4 zip holds only IF images S4A–S4L), so it would also fail criterion 2. C7: no per-replicate numbers. |

No other western blot is quantified (main figures, then Figures S1–S6).

## Decision

**Excluded, criterion 1.** The article quantifies western blots (Fig. 3E, Fig. S4N), but
no panel normalises a target band to a loading control: Fig. 3E is scaled to the
time-course maximum with no loading control stated, and Fig. S4N is a co-IP normalised
within the bait IP. No panel fits the unit of analysis under the Amendment 2 rules.
Under any reading the candidate is excluded, because neither panel has a deposited image
(criterion 2). The deposited raw blots (Figs. 2A, 6E, S1A, S2E, S3B) belong to
representative, unquantified panels.

No `fetch.sh` or `claim.yaml` was written. `images/` and `archives/` still hold the files
downloaded by `screen_files.py`.
