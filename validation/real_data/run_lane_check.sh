#!/usr/bin/env bash
# Lane detection on real 16-bit acquisitions, compared with the lanes the
# authors drew in Bio-Rad Image Lab (saved inside the .scn files).
#
# Usage: validation/real_data/run_lane_check.sh WORK_DIR
set -euo pipefail
work=${1:?usage: run_lane_check.sh WORK_DIR}
here=$(cd "$(dirname "$0")" && pwd)
bv="$here/../../build/apps/blotverse"
mkdir -p "$work"

# Harvard Dataverse doi:10.7910/DVN/YND7FA (CC0): "Figure 2 C pJNK.scn", ChemiDoc, 12 lanes + ladder.
[[ -f "$work/pJNK.scn" ]] || curl -sL -o "$work/pJNK.scn" https://dataverse.harvard.edu/api/access/datafile/6907926
# EMBO SourceData S-SCDT-10_1038-S44321-025-00317-0 (CC BY 4.0): pSTAT1, 14 lanes, half without signal.
[[ -f "$work/pSTAT1.scn" ]] || curl -sL -o "$work/pSTAT1.scn" \
  https://ftp.ebi.ac.uk/biostudies/fire/S-SCDT-10_1038-S44321-025-00317-/000/S-SCDT-10_1038-S44321-025-00317-0/Files/pSTAT1_quantif.scn

check() {  # name file lanes target_y reference_y [lanes_x]
  local name=$1 file=$2 n=$3 t=$4 r=$5 lx=${6:-}
  python3 "$here/imagelab_lanes.py" "$file" > "$work/$name.imagelab.tsv"
  if [[ -n "$lx" ]]; then
    "$bv" lanes "$file" --lanes "$n" --target-y "$t" --reference-y "$r" --lanes-x "$lx" > "$work/$name.blotverse.tsv"
  else
    "$bv" lanes "$file" --lanes "$n" --target-y "$t" --reference-y "$r" > "$work/$name.blotverse.tsv"
  fi
  printf '%-28s ' "$name"
  python3 "$here/compare_lanes.py" "$work/$name.imagelab.tsv" "$work/$name.blotverse.tsv"
}

check "pJNK (no hint)" "$work/pJNK.scn" 12 322 355
check "pSTAT1 (no hint)" "$work/pSTAT1.scn" 14 1055 1240
check "pSTAT1 (lanes_x hint)" "$work/pSTAT1.scn" 14 1055 1240 580,1723
