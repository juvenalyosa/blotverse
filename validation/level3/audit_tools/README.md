# Human-audit package

The auditor is not asked to run anything. These scripts turn the reconciled annotations into a
Word document with the annotation boxes drawn on the raw images and tick tables to fill in.

1. `overlay.py <experiment dir> <out dir>` (needs numpy, tifffile, pillow, pyyaml; run with
   `python -I`): writes `blot*_target_zoom.png` and `blot*_reference_zoom.png`. Boxes are the
   windows BlotVerse measures (`lane_rect` / `band_window` in `src/imaging/detect.cpp`).
2. Per experiment, `<out dir>` also needs `article_panel.png` (the published panel, cropped
   from `article/`, with no author values) and `lanes.txt` (one group per lane, from
   `experiment.yaml`).
3. `node build_audit_docx.js <pkg dir> <out.docx>` (npm `docx` 9.5.1) writes the document into
   `audit_package/` (gitignored: it holds third-party images).

The auditor's answers are transcribed into `<doi-slug>/audit.md`.
