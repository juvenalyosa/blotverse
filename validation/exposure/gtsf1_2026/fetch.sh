#!/usr/bin/env bash
# Two cumulative exposure series of one anti-GTSF-1 western blot each (Bio-Rad
# ChemiDoc, native .scn), from
#   "An Evolutionarily Conserved N-terminal Domain of RRF-3 Governs GTSF-1
#   Binding in Nematodes", EMBO Reports 2026, doi:10.1038/s44319-026-00914-1
# EMBO SourceData S-SCDT-10_1038-S44319-026-00914-1 (CC BY 4.0).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
base="https://ftp.ebi.ac.uk/biostudies/fire/S-SCDT-10_1038-S44319-026-00914-/001/S-SCDT-10_1038-S44319-026-00914-1/Files"
for f in 08.03.anti_gtsf1_1sec.scn 08.03.anti_gtsf1_5sec.scn 08.03.anti_gtsf1_50sec.scn 08.03.anti_gtsf1_100sec.scn \
         08.03.anti_gtsf1_150sec.scn 08.03.anti_gtsf1_200sec.scn \
         anti_GTSF1_5secauto.scn anti_GTSF1_40sec.scn anti_GTSF1_75sec.scn anti_GTSF1_110sec.scn anti_GTSF1_150sec.scn; do
  [[ -f "$here/$f" ]] || curl -sfL -o "$here/$f" "$base/$f"
done
echo "fetched into $here"
