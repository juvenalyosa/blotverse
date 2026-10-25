#!/usr/bin/env bash
# Two HEK IP western blots (PLC/tubulin and FLAG/tubulin), each captured at
# 5 s and 20 s (16-bit chemiluminescence channel exports, "_Ch"), from
# EMBO J 2025, doi:10.1038/s44318-025-00600-3; EMBO SourceData
# S-SCDT-10_1038-S44318-025-00600-3 (CC BY 4.0).
set -euo pipefail
here=$(cd "$(dirname "$0")" && pwd)
base="https://ftp.ebi.ac.uk/biostudies/fire/S-SCDT-10_1038-S44318-025-00600-/003/S-SCDT-10_1038-S44318-025-00600-3/Files"
get() { [[ -f "$here/$2" ]] || curl -sfL -o "$here/$2" "$base/$1"; }
get hek_ip_plctub_5s_2023.09.13_11.24.53_Ch.tif plc_5s.tif
get hek_ip_plctub_20s_2023.09.13_11.26.27_Ch.tif plc_20s.tif
get hek_ip_wt_flagtub_5s_2023.09.13_11.19.59_Ch.tif flag_5s.tif
get hek_ipwt_flagtub_20s_2023.09.13_11.22.14_Ch.tif flag_20s.tif
echo "fetched into $here"
