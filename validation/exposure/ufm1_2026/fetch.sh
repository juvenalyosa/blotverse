#!/usr/bin/env bash
# Anti-UFM1 western blots, each captured at a normal and a lower exposure
# (ChemiDoc 16-bit exports; exposure times not recorded in the files), from
# EMBO J 2026, doi:10.1038/s44318-026-00753-9; EMBO SourceData
# S-SCDT-10_1038-S44318-026-00753-9 (CC BY 4.0).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
base="https://ftp.ebi.ac.uk/biostudies/fire/S-SCDT-10_1038-S44318-026-00753-/009/S-SCDT-10_1038-S44318-026-00753-9/Files"
for f in WB_UFM1.tif WB_UFM1_lower_exposure.tif WB_UFM1_replicate_2.tif WB_UFM1_lower_exposure_replicate_2.tif \
         WB_UFM1_replicate_2_1.tif WB_UFM1_lower_exposure_replicate_2_1.tif \
         WB_UFM1_replicate_3_1.tif WB_UFM1_lower_exposure_replicate_3_1.tif; do
  [[ -f "$here/$f" ]] || curl -sfL -o "$here/$f" "$base/$f"
done
echo "fetched into $here"
