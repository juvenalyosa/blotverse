# Screening notes: 10.1016/j.cryobiol.2018.07.004

"Adaptation of the FK506 binding protein 1B to hibernation in bats", *Cryobiology* 83:1-8
(2018). PMID 30056853, PII S0011-2240(18)30112-3. Second frame, rank 16. Deposit: Mendeley
Data x8c995yd4s v1 ("Western Blots of YY1, SP1, and FKNP1B"): `FKBP1B.zip`, `SP1.zip`,
`YY1.zip`, `Supplementary Excel file 1-3.xlsx`. Screened 2026-10-27 under the Brief with the
Amendment 2 addition.

**Decision: not reached (screening incomplete). The article's full text could not be
obtained; criterion 1 and the panel list cannot be assessed.**

## Sources

* `article/elsevier_api_metadata.xml`: Elsevier article API response without a key
  (metadata only; `openaccess` 0, `openArchiveArticle` false). SHA-256
  322c551d9b253f8726c9c12f575878b31c45d3f9647c88af157f0fca787474a1.
* `article/deposit_metadata.json`: Mendeley public API record of the deposit.
* Europe PMC: abstract only, no PMCID. OpenAlex: `oa_status` closed, no repository copy.
  ScienceDirect refuses automated access (HTTP 403); this was not worked around.

## What is known without the full text

* Peer-reviewed journal article, not a preprint.
* Abstract: "The expression of the Fkbp1b gene was found to be elevated at both mRNA and
  protein levels in two distantly related bats (*Rhinolophus ferrumequinum* ... and *Myotis
  ricketti* ...) during torpor." This implies a western blot comparison (torpor vs. other
  states), but whether it is quantified, normalised to a loading control, its n and its
  groups need the figures and legends.
* Deposit `FKBP1B.zip` (already in `archives/`; SHA-256 4a280442...): folders `M/` (8
  chemiluminescence TIFFs `2016MMDD_HHMM_15.tif`, 7 Ponceau images) and `R/` (5
  chemiluminescence TIFFs, 5 Ponceau images, and `Sequence*.txt` / `se.txt` files listing lane
  order, e.g. "R-I-1 R-II-1 R-III-1 R-IV-1 rat ..."). `blotverse inspect` on all 25 TIFFs:
  1536 x 1024, 16 bits, 16 effective bits, raw = yes (several R/ images have saturated
  pixels: 1035-3322). Lanes are vertical (preview of `R/20160604_1753_15.tif`).
* The lane labels suggest four groups (I-IV) of three animals for *R. ferrumequinum*, plus a
  rat lane, with a Ponceau total-protein image per membrane. What I-IV denote is not stated in
  the deposit metadata or file names; the article is needed for criterion 5. The `M/` folder
  has no lane-order file.
* The three Excel files were not opened (they may hold the authors' numbers; criterion 7 is
  reached only after a panel passes criteria 1-6 and 8).

## Panels examined

None: the panel list (step 2 of the Brief) needs the figures and legends.

## To finish this screen

Obtain the article PDF (institutional access to *Cryobiology*, or from the authors), save it
in `article/`, and screen the FKBP1B / YY1 / SP1 western blot panels in figure order. The
deposit images are raw, so criterion 2 is likely to pass for FKBP1B; criteria 3-6 depend on
the legends (groups I-IV, n per group, Ponceau as loading control).
