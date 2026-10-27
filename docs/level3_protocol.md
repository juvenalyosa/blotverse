# Level 3: reanalysis of published western blots — protocol

Status: **v1.0, frozen** in commit 377b2a4 (2026-10-27), together with
[`validation/level3/annotation_prompt.md`](../validation/level3/annotation_prompt.md). It is
registered on OSF together with the spec before the candidate frame is built.
OSF registration: *pending*.

No candidate image may be downloaded before the commit that freezes this file. The two
pilot cases in [level3.md](level3.md) were processed before this protocol and are not part
of the sample.

Plan section 10 and hypothesis H3 (exploratory): *a substantial fraction of published
western blot quantifications with deposited raw images is fragile.*

## What is measured

For a sample of published experiments, each with raw, uncropped images and the authors'
own quantification:

* **Technical:** does BlotVerse measure what the authors measured?
* **Scientific:** does the authors' conclusion survive every defensible analysis in the
  frozen multiverse?

H3 is exploratory. No threshold decides it: every endpoint is an estimate with its 95 %
interval.

## Sample: design-agnostic (decision of 2026-10-27)

**Experiments are included whatever their design.** The design does not need to make the
joint test evaluable. A typical published design is 3 blots x 1 lane per group. It allows
2^3 = 8 within-blot relabellings, so the joint test cannot fall below 2/8 = 0.25 and the
verdict is *Not evaluable*. Excluding such designs would restrict the sample to unusually
large experiments, and the result would not describe published practice.

Consequences:

* **Primary analysis:** every included experiment. It uses the quantities that need no
  joint test: R, direction, effect range and influence. The share of non-evaluable designs
  is itself an endpoint.
* **Secondary analysis:** the verdict (Robust / Fragile / Not supported), only in the
  experiments whose design is evaluable.
* **Stopping rule:** the sample is not extended to collect more evaluable designs. A
  targeted search for evaluable designs is allowed only as a separately declared
  supplementary sample, never pooled with the main one.

## Unit of analysis

**One experiment per article.** An experiment is a western blot panel in which:

* the authors normalise a target band to a loading control (a housekeeping protein or
  total protein);
* they compare ≥ 2 groups;
* they report a quantification and a conclusion.

In each article, the experiment is the **first panel in figure order** (main figures, then
extended or supplementary figures) that meets every inclusion criterion. Later panels are
not examined. One experiment per article keeps laboratories from weighing in more than
once and leaves no choice of panel to the annotator.

**Primary contrast:** the first contrast of that panel stated in the text or legend. For
example, "loss of X reduces Y" gives *knockout vs control*. Other contrasts in the panel
are analysed and reported as secondary. The control group is the group the authors compare
against (control, wild type, vehicle).

## Sampling frame

Built **after** this protocol is frozen and **before** any image is downloaded, from
repository metadata only.

| Repository | Query (deposit date 2020-01-01 to the freeze date) |
|---|---|
| EMBO SourceData (BioStudies `S-SCDT-*`) | source-data files with extension `.scn`, `.gel`, `.tif` or `.tiff` whose name or panel description contains "blot" or "WB" |
| Zenodo | `"western blot" AND (raw OR uncropped OR original)`, resource type dataset |
| Mendeley Data | `"western blot" raw images` |
| Figshare | `"western blot" raw` (datasets) |
| Dryad | `"western blot"` |
| Harvard Dataverse, UNC Dataverse | `"western blot"` (datasets) |

* Each hit that links to a peer-reviewed article becomes one candidate.
* Several deposits of one article are merged into one candidate.
* Candidate identifier: the article DOI.
* The frame is saved as `validation/level3/candidates.csv` with these columns: DOI,
  repository, dataset identifier, query, date searched.
* The frame is committed before screening starts.

**Excluded from the frame** (used during development; every article behind these
datasets):

* EMBO SourceData:
  * S44319-026-00850-0, S44318-026-00753-9, S44319-026-00914-1, S44318-025-00572-4,
    S44321-025-00317-0, S44319-025-00521-6;
  * every study listed in [`validation/exposure/screening.md`](../validation/exposure/screening.md).
* UNC Dataverse doi:10.15139/S3/4ESRC1.
* Harvard Dataverse doi:10.7910/DVN/YND7FA.
* Zenodo 10050244, 13752216, 18299536 and 4598136.
* Figshare 10.6084/m9.figshare.4516847.
* Mendeley czm3p6t2xk and vwcy56z5jh (Pillai-Kastoori 2020: a dilution series, not a
  group comparison).

**Screening order.** Candidates are screened in ascending order of
`SHA-256("blotverse-level3-v1:" + lower-case DOI)`. Neither the order nor the
identity of the next candidate can be chosen.

**Stopping rule.**

* Screening stops when **50** experiments are included or the frame is exhausted.
* If the frame yields fewer than **30**, a second frame (more repositories or an earlier
  start date) may be added. It is added by amendment, before any of its images are
  downloaded.

With 50 experiments, a proportion is estimated within about ±0.14 (95 % Wilson interval at
p = 0.5); with 30, within about ±0.18.

## Inclusion criteria

Criteria are checked in this order, and the first one that fails is recorded as the
reason for exclusion.

1. **Peer-reviewed article** with a western blot panel quantified by the authors.
2. **Raw acquisition readable by BlotVerse without rejection.** The file must be one of:
   * a native imager file (`.scn`, `.gel`);
   * a 16-bit or floating-point greyscale TIFF.

   8-bit exports, upscaled 8-bit data, RGB or pseudo-colour files and JPEG are excluded:
   they are usually display-adjusted, and saturation cannot be checked on them. The check
   is `blotverse inspect FILE...` (column `raw`).
3. **Uncropped.** The deposited image shows every lane of the quantified replicates.
4. **≥ 3 biological replicates per group**, as stated by the authors. Every replicate must
   be present in the deposited images, for both the target and the loading control.
5. **Lanes, groups and replicates identifiable** from the article, the legend, the deposit's
   metadata, file names or an overview figure, **without using the authors' numbers**.
   Matching lanes to the authors' values to infer the mapping is circular, and an
   ambiguous mapping excludes the experiment.
6. **Loading control identifiable** for every replicate: a housekeeping band, or a
   total-protein image of the same membrane.
7. **Authors' quantification per replicate** in numbers: per-lane signals, or normalised
   values per replicate (source-data tables, xlsx, csv). A bar chart alone is not enough.
8. **Vertical lanes** in the image, or lanes made vertical by a 90-degree rotation. Other
   rotations are not supported.

The joint test does not need to be evaluable (see *Sample*).

All screening goes into `validation/level3/screening.md`, using the format of the exposure
screening, with one row per candidate. Excluded candidates appear in the flow diagram.

## Annotation

**Annotation is done by AI agents and audited by a person.** This is stated in every report
of the study. Two annotators work independently:

* **Annotator A:** an AI agent on Claude Opus.
* **Annotator B:** an AI agent on a different model (Claude Sonnet), started in a separate
  context, with no access to annotator A's files or reasoning. Two runs of one model are not
  independent annotators: they would share the same errors and inflate the agreement.
* The exact model identifiers and the annotation prompt are recorded in
  `validation/level3/annotation_prompt.md` and committed with the freeze. The prompt does
  not change during the study.
* Each agent writes, next to its annotation, a log (`annotator_a.md`, `annotator_b.md`).
  The log gives, for each lane, the evidence for its group and replicate: a figure, a
  legend, a file name or metadata.

**Blinding** is enforced by the file layout. `fetch.sh` puts the authors' quantification
files (source-data tables, xlsx, csv) in `authors/`. Annotator agents are not given that
folder, and their logs must not cite it.

**Human audit.** Jorge Leyva, PhD, checks a random 20 % of the included experiments, at least
6 of them:

* **Selection:** the experiments whose `SHA-256("blotverse-level3-audit:" + DOI)` falls
  in the lowest fifth.
* **Method:** the auditor checks the reconciled annotation against the article, without
  the authors' numbers. Each lane-to-group mapping and band row is graded *correct*,
  *minor error* (geometry only) or *wrong* (mapping).
* **Endpoint:** see A4. No claim of superiority over human annotation is made. The audit
  measures whether AI annotation is acceptable.

**Before annotating**, the authors' **claim** for the primary contrast is recorded in
`claim.yaml`:

* direction: up, down or no difference;
* the authors' p-value or significance mark;
* n per group;
* the quoted sentence.

**What each annotator does:**

* Writes a BlotVerse experiment file for each experiment, containing:
  * lanes, groups and blots;
  * band rows;
  * the lanes (and, if needed, the band rows) of the separate loading-control image, if
    any, under `reference_image` / `reference_annotation`;
  * `invert` where the image has dark bands.
* May use `blotverse detect`, `blotverse lanes` and `validation/level3/refine_lanes.py`.
* Is **blind to the authors' numbers.** They may read the article, legends and overview
  figures, but not the source-data tables.
* Only annotates lanes of the experiment's groups. Ladders, inputs, IgG and other
  unrelated lanes are left out.

**Saving and checking:**

1. Annotations are committed (`annotator_a.yaml`, `annotator_b.yaml`) before BlotVerse is
   run on them.
2. Where the annotators disagree:
   * **Lane geometry** (centres, widths, band rows): settled by the mean of the two.
   * **Lane-to-group mapping:** settled by the human auditor, from the article and both
     logs, without the authors' numbers.

   The result is `experiment.yaml` (the reconciled annotation), committed before the run.
3. If the mapping cannot be settled, the experiment is excluded under criterion 5. That
   exclusion is counted separately in the flow diagram.

**After the run, an annotation is not changed** except to fix a factual error, for example
a wrong file or a lane off by one. Such fixes are:

* logged with the reason;
* reported with the results of both versions.

## Analysis

* **Software:**
  * BlotVerse at the freeze commit;
  * spec v1.3.0 (`spec/multiverse.yaml`, sha256
    `b13c69ec862eec1610bdab912bd96b512c88ca4cf8cc110492e1c1db72185d24`);
  * spec defaults for permutations and seeds.
* **Command:** `blotverse run experiment.yaml --out <dir>`. The results and `repro.yaml`
  of every experiment are kept in `validation/results/level3/<doi-slug>/`.
* **QC:**
  * Lanes that QC excludes (for example saturated bands) stay excluded, as in any run.
  * An experiment left with < 3 lanes in some group after QC is analysed and flagged.
  * The share of experiments with QC exclusions is reported.
* **Authors' values:** go into `authors.csv`. They are read only after the run.

## Endpoints

Primary analyses use all included experiments and the primary contrast, with 95 % Wilson
intervals for proportions and bootstrap intervals (percentile, 10 000 resamples of
experiments, seed 20261027) for medians.

### Technical

* **T1. Per-replicate fold change.**
  * For each replicate, compute the log2 fold change of the primary contrast, normalised
    to the loading control: once from the authors' numbers and once from the default
    universe.
  * Report:
    * the median absolute difference;
    * the fraction of replicates with the same sign;
    * the correlation across all replicates of all experiments.
* **T2. Per-lane signal agreement.** Only where the authors give per-lane signals: the
  Pearson r of log signals between the authors and each background x band-definition
  branch.
  * The branch closest to the authors' method is reported as descriptive.
  * It is not chosen to maximise agreement.

### Scientific: primary (all experiments)

* **S1. Non-evaluable designs.** Fraction of experiments whose joint test cannot reach
  alpha (2 / relabellings ≥ 0.05).
* **S2. Direction.** Per experiment: the fraction of valid universes whose effect has the
  sign of the default universe. Report:
  * its median;
  * the fraction of experiments where it is 1 (unanimous);
  * the fraction where it is ≥ 0.9.
* **S3. Robustness score R.** Report:
  * the distribution across experiments (median, IQR);
  * the fractions with R ≥ 0.9, 0.5 ≤ R < 0.9 and R < 0.5.

  These classes use the verdict's R thresholds. Without the joint test they are not
  verdicts and are not reported as such.
* **S4. Effect spread.** Width in log2 of the 5–95 % range of the effect across universes.
  Report its median across experiments.
* **S5. Influence.** For each decision:
  * the number of experiments in which it is the most influential;
  * its median eta².
* **S6. Agreement with the published claim.**
  * Does the default universe agree with the authors' claim? Agreement means:
    * same sign and p < 0.05 when the authors claim an effect;
    * p ≥ 0.05 when they claim no difference.
  * Fraction of universes that agree with the claim, per experiment, and its median.

### Scientific: secondary (evaluable designs only)

* **V1.** Distribution of verdicts (Robust / Fragile / Not supported), with Wilson
  intervals.
* **V2.** Among experiments whose authors claim an effect: the fraction Fragile or Not
  supported. This is the direct estimate for H3, limited to designs that can be evaluated.

### Annotation agreement

* **A1.** Fraction of experiments whose two annotations map lanes to groups identically.
* **A2.** ICC(2,1), absolute agreement, of the per-lane log2 target / loading ratio in the
  default universe between the two annotations.
* **A3.** Agreement between the annotators' runs in:
  * the S2 direction class;
  * the S3 R class;
  * the verdict, in evaluable designs.
* **A4. Human audit.** In the audited experiments: the fraction of lane-to-group mappings
  graded *wrong* and the fraction with any *minor error*, with Wilson intervals. Every
  *wrong* mapping is fixed in `experiment.yaml` before that experiment is analysed. An
  experiment audited after its run is rerun and reported in both versions.

### Sensitivity

* S1–S6 and V1 rerun on each annotator's own annotation.
* S2–S6 restricted to experiments with ≥ 3 lanes per group after QC.

## Limitations stated in advance

* **Selection.** Authors who deposit raw 16-bit images are probably more careful than
  average, so fragility may be underestimated (plan section 10). The 16-bit requirement
  makes the sample even more selective.
* **The share of non-evaluable designs depends on the frame.** It describes deposited,
  quantified experiments, not all published western blots.
* **Unknown author processing.** The authors' quantification may use a method or region
  that no branch reproduces. In that case T1/T2 measure agreement with a method, not with
  truth.
* **AI annotation.** Both annotators are AI models from one provider and may share
  systematic errors that their agreement (A1–A3) cannot reveal. The human audit (A4)
  bounds this on a sample only.
* **No integrity screening.** `blotverse forensics` is not run on level-3 images. The
  study is about methods, not conduct.

## Reporting and ethics

* Results are reported in aggregate. Per-experiment results are kept in the repository
  for reproducibility, without judgement of individual articles.
* The main text names no article as fragile.
* Sources are listed in a supplementary table, with their licences.
* Deviations from this protocol are logged in an *Amendments* section with:
  * the date;
  * what had been seen when the deviation was made.

## Files

```
validation/level3/
  annotation_prompt.md    models and prompt of the annotator agents (frozen)
  candidates.csv          sampling frame (frozen before screening)
  screening.md            one row per screened candidate, reason for exclusion
  <doi-slug>/
    fetch.sh              downloads the deposited files
    claim.yaml            authors' claim (recorded before annotation)
    article/              article PDF, figures, legends, overview files (annotators read these)
    images/               deposited raw images
    authors/              authors' quantification files (annotators never open these)
    annotator_a.yaml      independent annotations, with their logs
    annotator_a.md
    annotator_b.yaml
    annotator_b.md
    experiment.yaml       reconciled annotation (committed before the run)
    audit.md              human audit, for the audited experiments
    authors.csv           authors' per-replicate values (filled after the run)
validation/results/level3/<doi-slug>/   BlotVerse output and repro.yaml
```

## Amendments

### Amendment 1 (2026-10-27): freeze record and operational search rules

Made before the frame was built. At that point only the hit counts and the first few titles
returned by test queries had been seen; no candidate's files had been opened.

**1. Freeze.** The protocol was frozen in commit 377b2a4 and published on GitHub
(`level3-protocol` branch) on 2026-10-27. The public commit fixes the freeze date. The OSF
registration of the same files (same SHA-256) followed after the frame was built,
contrary to the order stated under *Status*.

**2. Operational search rules.** The queries above are applied through each repository's
public API by `validation/level3/build_frame.py`, which also writes the search log
`frame_log.md`. Three problems turned up when the queries were tested:

* Mendeley Data and Figshare ignore the quotation marks and combine words with OR. The
  query `"western blot" raw images` returned 22 458 Mendeley records, almost all unrelated.
* Figshare honours the phrase only with the title qualifier (`:title: "western blot"`).
  With `:description:`, or with no qualifier, it returns the whole catalogue in date order.
  Figshare is therefore searched by title only.
* UNC Dataverse refuses unauthenticated searches (HTTP 401).
* No repository offers "links to a peer-reviewed article" as a search filter.

The following rules therefore apply to every repository except EMBO SourceData:

* **Query.** Searches use the phrase `"western blot"`. The protocol's extra Zenodo terms
  (raw OR uncropped OR original) are kept, because Zenodo honours them.
* **Text filter.** A hit is kept only if "western blot" (spaces, hyphen or underscore
  allowed) appears in its title, description or categories.
* **Date filter.** Its publication date must fall between 2020-01-01 and 2026-10-27.
* **Article link.** The hit links to an article when its metadata carries an article DOI.
  The DOI may come from:
  * a related identifier, related article or publication field;
  * failing that, the first DOI in its description that is not a data-repository DOI.

  Hits without an article DOI do not enter the frame.

**EMBO SourceData** studies come from the SourceData collection search for `blot` and
`WB`. A study enters the frame when it was released in the date window and at least one
of its `.scn`, `.gel`, `.tif` or `.tiff` files has "blot" or "WB" in its name or in the
description of its figure panel. The article DOI is the one encoded in the accession.

**UNC Dataverse** is searched when the API allows it. When it refuses, the refusal is
logged in `frame_log.md` and UNC contributes no candidates.
