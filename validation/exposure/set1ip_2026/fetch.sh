#!/usr/bin/env bash
# One anti-GFP western blot (GFP-Set1 IP) captured at 20 s and 294.7 s
# (Bio-Rad ChemiDoc raw16 TIFF exports), from EMBO Reports 2026,
# doi:10.1038/s44319-026-00901-6; EMBO SourceData
# S-SCDT-10_1038-S44319-026-00901-6 (CC BY 4.0).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
base="https://ftp.ebi.ac.uk/biostudies/fire/S-SCDT-10_1038-S44319-026-00901-/006/S-SCDT-10_1038-S44319-026-00901-6/Files"
get() { [[ -f "$here/$2" ]] || curl -sfL -o "$here/$2" "$base/$1"; }
get "GFPSet1IP-GFPBlot-1_Chemiluminescence_20.000s_2025-12-02_14h44m24s.raw16.tif" gfp_20s.tif
get "GFPSet1IP-GFPBlot-2_Chemiluminescence_294.734s_2025-12-02_14h49m11s.raw16.tif" gfp_295s.tif
echo "fetched into $here"
