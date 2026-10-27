# Screening: 10.1016/j.molcel.2017.04.024

Second sampling frame, rank 139. Screened under the brief with the Amendment 2 addition
(criterion 7 records only; phospho/total accepted as loading control; co-IP bait is not).

Tsuchiya, Ohtake, Arai, Kaiho, Yasuda, Tanaka and Saeki (2017) "In Vivo Ubiquitin
Linkage-type Analysis Reveals that the Cdc48-Rad23/Dsk2 Axis Contributes to K48-Linked Chain
Specificity of the Proteasome", *Molecular Cell* 66:488-502 (peer-reviewed research article;
PMID 28525741; not in PMC or Europe PMC full text). Deposit: Mendeley Data t4xtrtj2gp v3
(doi:10.17632/t4xtrtj2gp.3), 209 single-image TIFFs named by figure panel and antibody
("Figure S7D IB-Cdc48 .tif", ...). No source-data files in the deposit.

Sources read (in `article/`): Elsevier "Document S2. Article plus Supplemental Information"
(`article_plus_SI_mmc8.pdf`, full text, main legends, STAR Methods, supplementary figures and
legends), Document S1 (`DocumentS1_mmc1.pdf`), Mendeley API metadata
(`deposit_metadata.json`). The publisher HTML page (cell.com / sciencedirect) refuses scripted
access (HTTP 403). Open-access lookup APIs that require an e-mail (Unpaywall, Crossref/OpenAlex "mailto")
were not used, and no personal identifier was sent to any service. The PDFs came from the
publisher's static file server (ars.els-cdn.com), which serves them directly. Tables S1-S6
(`mmc2.xlsx`-`mmc7.xlsx`) hold the authors' numbers and are in `authors/`.

Most quantification in the article is mass spectrometry (Ub-AQUA/PRM), not western blot. The
STAR Methods "Quantitative Immunoblots" name two blot quantifications (Fig. 2 IP standard
curve; Fig. 3D Vps27-3xFLAG normalised to Pgk1); the supplementary legends add S1C, S4E, S7D.

## Western blot panels, in figure order

| Panel | Criterion failed / result | Evidence |
|---|---|---|
| Fig. 1B, 3A, 5, 6, 7A/E | not quantified | Immunoblots of IPs, BIOTUBE pull-downs and in vitro GST pull-downs (Ponceau S); no blot quantification in figures or legends. Fig. 2 and 4 contain no blots; the bar charts in Fig. 2, 3B/D, 4 and 7 are Ub-AQUA/PRM (fmol) values. |
| Fig. 3C (Vps27-3xFLAG IP, WT / hse1Δ / vps4Δ) | not an experiment (would fail 4) | Numbers 1.0 / 1.0 / 1.3 printed under the anti-FLAG IP lanes; Methods: Vps27-3xFLAG "quantified by anti-FLAG immunoblot and was normalized to Pgk1 signal", used as a normalisation factor for the MS data of Fig. 3D. No comparison or conclusion is reported for these values, and the band is immunoprecipitated bait (Pgk1 is only in the input lanes). C2 passes (`Figure 3C IB-FLAG.tif` 900x796 and `IB-Pgk1 .tif` 822x829, 16-bit, raw = yes), but the deposit holds a single set of 4 input + 4 IP lanes: if it counted as a quantified panel it would fail criterion 4 (no replicates). |
| Fig. S1A (FLAG / Pgk1, 14 tagged strains) | not quantified | Expression blot, Pgk1 "used as the loading control", no quantification. |
| Fig. S1C (Rad23, Dsk2 / Pgk1, WT vs 3xFLAG knock-in; n = 3) | **2** | Quantified ("relative abundance (normalized)", mean ± SEM, three biologically independent experiments; scheme S1B: "WB (normalized to Pgk1)"). No image file for S1C in the deposit: the Figure S1 files are S1A (FLAG, Pgk1), S1D/S1F plates, S1E (Dsk2, Rad23, Pgk1, Ub of WT / DSK2-3xFLAG / 3xFLAG-DSK2, one set, no RAD23-3xFLAG lane) and microscopy. No raw acquisition of the quantified blots exists to inspect. |
| Fig. S2, S3, S5, S6 | not quantified | Blots shown with Ub-AQUA/PRM or pull-downs; S3E/F text quantification is MS. |
| Fig. S4E (Tat2-GFP / Pgk1, BTZ 0/2/4/6 h; n = 3) | **3** | C2 passes: `Figure S4E IB-GFP.tif` 501x829 and `IB-Pgk1.tif` 486x841, 16-bit, effective 16 bit, raw = yes. C3 fails: each deposited image is one membrane with one set of 5 sample lanes (no-plasmid, 0, 2, 4, 6 h) = one replicate; the other two of the "three biologically independent experiments" are not deposited. |
| **Fig. S7D (Cdc48 / Pgk1, WT vs cdc48-3; n = 3)** | **included** | See below. |

### Fig. S7D, criterion by criterion

* **Unit of analysis.** Legend: "Signal intensities of Cdc48 were quantified and normalized to
  Pgk1 and represented as means ± SEM of three biologically independent experiments"; two
  groups (WT, cdc48-3); conclusion in Results: "The mutant protein was expressed normally"
  (Figures S7D and S7F). Pgk1 is a housekeeping loading control.
* **C2 pass.** `blotverse inspect images/FigS7D_IB-Cdc48.tif images/FigS7D_IB-Pgk1.tif`:
  918x1024 and 1080x574, 16 bit, effective 16 bit, 15165 and 15562 grey levels, 254 and 49
  saturated px, raw = yes (warning: bright background, set `invert: true`).
* **C3 pass.** Both images are the whole strips with the marker lane at the left (handwritten
  "αCDC", "αPGK") and empty membrane to the right; nothing cut off.
* **C4 pass.** Legend n = 3 per group. Each strip carries six sample lanes in three pairs
  separated by an empty lane: 3 × (WT, cdc48-3), for target and loading control.
* **C5 pass.** The published crop (article/FigS7D_overview.png, labels only, no values) shows
  two adjacent lanes labelled WT, cdc48-3 from left to right. The deposit has exactly 3 + 3
  lanes, laid out as three separated pairs, which fits three independent experiments of the
  pair WT | cdc48-3. Group order within each pair is taken from the crop (WT left). This
  layout is inferred from the figure and the gel structure, not from the authors' numbers. The
  replicate index of each pair (1-3) is arbitrary. The Cdc48 signal does not differ visibly
  between lanes, so the mapping was not guessed from the outcome. Caveat for the reconcilers:
  the article does not state the lane order of pairs 2 and 3. While opening Table S6 to
  confirm criterion 7, I saw that its column headers (not values) follow the same WT, cdc48-3
  order per experiment. That observation is not used as evidence here.
* **C6 pass.** Pgk1 strip of the same lysates, same six-lane pair layout (lanes at matching x
  positions), one band per lane. It is a separate image of a different size, so it goes in
  as `reference_image`.
* **C7 (records only, Amendment 2): per-replicate numbers exist.** `authors/mmc7.xlsx`
  (Table S6), sheet "6D", has per-lane Cdc48 and Pgk1 signals and the normalised values for
  experiments #1-#3, WT and cdc48-3. I read only the labels; the values are masked.
* **C8 pass.** Lanes are vertical (bands horizontal), no rotation needed.

## Decision

**Included: Fig. S7D** (Cdc48 normalised to Pgk1, cdc48-3 vs WT, n = 3 per group, direction
"no difference", no p-value reported). The earlier quantified panels fail as follows: Fig. S1C
on criterion 2 (blot images not deposited) and Fig. S4E on criterion 3 (one of three replicates
deposited). Fig. 3C does not count as an experiment because it is an IP normalisation factor
with no reported comparison.

Prepared: `fetch.sh` (verified in a clean directory, all hashes OK, overview render
reproducible), `claim.yaml`, `article/`, `images/` (only the two S7D TIFFs; the frame-2
screen downloads `Figure 1B left panel IB-Ubiquitin .tif` and `Figure 5A Ponceau S.tif` were
removed), `authors/` (Tables S1-S6).
