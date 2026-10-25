#!/usr/bin/env bash
# Raw images and the authors' quantification for Fig. 3G of
#   AURORA A - DICER (EMBO Reports 2026, doi:10.1038/s44319-026-00850-0)
# from EMBO SourceData (BioStudies S-SCDT-10_1038-S44319-026-00850-0, CC BY 4.0).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
base="https://ftp.ebi.ac.uk/biostudies/fire/S-SCDT-10_1038-S44319-026-00850-/000/S-SCDT-10_1038-S44319-026-00850-0/Files"
for f in 3G_WB_DICER_R1.tif 3G_WB_DICER_R2.tif 3G_WB_DICER_R3.tif 3G_WB_HA_R1.tif 3G_WB_HA_R2.tif 3G_WB_HA_R3.tif \
         3G_WB_overview.pdf 3G_WB_quantification.xlsx; do
  [[ -f "$here/$f" ]] || curl -sfL -o "$here/$f" "$base/$f"
done
echo "fetched into $here"
