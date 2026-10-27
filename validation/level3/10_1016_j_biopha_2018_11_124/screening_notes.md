# Screening notes: 10.1016/j.biopha.2018.11.124

Hui F, Qin X, Zhang Q, Li R, Liu M, Ren T, Zhao M, Zhao Q (2019), "Alpinia oxyphylla oil
induces apoptosis of hepatocellular carcinoma cells via PI3K/Akt pathway in vitro and in vivo",
Biomedicine & Pharmacotherapy 109:2365-2374. PMID 30551496 (no PMC copy).
Deposit: Mendeley Data mxmd7fkhnd v1, "Data for: Alpinia oxyphylla oil induces apoptosis of
Hepatocellular Carcinoma cells via PI3K/Akt pathway in vitro and in vivo", published
2018-11-30. One file, `western blot image.zip` (SHA-256 2bda32f3...c22053, matches the
Mendeley API record).

Screened 2026-10-27, second frame, under the brief with the Amendment 2 addition (criterion 7
does not exclude; phospho / total accepted; co-IP bait is not a loading control).

## Criterion 1

Peer-reviewed journal article. The full text could not be retrieved here: there is no PMC or
Europe PMC copy, the Elsevier API returned metadata only, and ScienceDirect refuses automated
access. The abstract (`article/europepmc_30551496.json`) reports western blot results with
significance per protein ("The expression levels of BAX (p < 0.01), cleaved caspase-9
(p < 0.01) and cleaved caspase-3 (p < 0.05) were increased significantly in the PE-treated
group at the dose of 1 g/kg ... Bcl-2 (p < 0.01) was decreased ..."; PTEN up, PI3K down,
p-Akt inhibited; cytochrome c released to the cytosol). So it has quantified western blots.
Passes.

## Deposit structure

The zip has one folder per protein, each with three replicate subfolders `1/`, `2/`, `3/`
holding the target image and its own β-actin image (`ª┬-actin-N.tif`, a mis-encoded "β"):
Bax, Bcl-2, Cleaved caspase-3, Cleaved caspase-9, Cytochrome c, P-Akt/Akt (P-Akt-N and Akt-N,
no actin), PI3K, PTEN. Each image is a cropped strip.

## Panels examined

The figure numbers could not be checked without the full text. The order of the abstract
(cytochrome c release, then Bax/Bcl-2/caspases, then PTEN/PI3K/p-Akt) is taken as figure
order; it does not matter for the decision, because every quantified protein fails
criterion 2 in the same way. `build/apps/blotverse inspect` (column `raw`) on every file of the
zip:

| Panel (protein) | First failing criterion | Evidence |
|---|---|---|
| **Cytochrome c (first WB panel by the abstract order)** | **2** | `cyt-1/2/3.tif` and `ª┬-actin-1/2/3.tif`: all 8-bit, raw = no (cyt-3 and its actin: 534 798 and 736 980 saturated pixels; actin-2: 123 grey levels) |
| Bax | 2 | all six files 8-bit |
| Bcl-2 | 2 | all six files 8-bit (Bcl-2-2 and actin-2 saturated) |
| Cleaved caspase-9 | 2 | all six files 8-bit |
| Cleaved caspase-3 | 2 | all six files 8-bit |
| PTEN | 2 | `PTEN-1.tif` colour (pseudo-coloured); all other files 8-bit |
| PI3K | 2 | replicate 1 (`PI3K-1`, actin-1) is 16-bit, raw = yes; replicates 2 and 3 are 8-bit |
| P-Akt / Akt | 2 | all six files 8-bit (Akt-1, Akt-2 with saturated pixels) |

Criterion 7 (not excluding): no tables were found in the deposit; the article's supplementary
material could not be checked.

## Decision

**Excluded, criterion 2 (first quantified western blot panel, cytochrome c; figure number not
verified).** Every quantified protein has 8-bit (or pseudo-colour) images for at least two of
its three replicates; no panel has raw 16-bit images of all replicates.

## Files

* `article/europepmc_30551496.json`: Europe PMC record with the abstract.
* `images/`, `archives/`: left as downloaded by `screen_files.py` (not curated, candidate
  excluded). The PI3K and PTEN files were extracted to a scratch directory for inspection only.
* No `fetch.sh` or `claim.yaml` (excluded). No file with the authors' numbers was saved.
* No API that needs an e-mail address was used or skipped (Europe PMC, Elsevier API without key, Mendeley public API, OpenAlex without a mailto parameter).
