#!/usr/bin/env bash
# Re-downloads every file used for 10.1016/j.biopha.2019.109053 (Fig. 3G/H, MHC / GAPDH,
# rat diaphragm, control vs cigarette smoke 4, 8, 12 weeks, n = 3 rats per group).
# article/  abstract (Europe PMC), Mendeley deposit metadata (lane order statement), figures
#           without quantification (Figs 1, 2, 5) and a crop of Fig. 3G (blot and lane labels only)
# images/   raw 16-bit TIFFs of the Fig. 3G membranes (MHC, MHC-I = BAF8, MHC-II = F59, GAPDH)
# authors/  every file with the authors' numbers (annotators never open this folder):
#           Figs 3, 4, 6, 7 (bar charts of the western blot quantification, with significance marks)
# The article full text and legends (ScienceDirect, PII S075333221931529X) returned HTTP 403 to
# scripted downloads during screening; get them by hand from https://doi.org/10.1016/j.biopha.2019.109053
# and put the PDF in authors/ (it contains the bar charts of Figs 3, 4, 6, 7).
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p article images authors archives

get() { # url dest sha256 (sha "-" = served dynamically, hash not checked)
  curl -sSL -o "$2" "$1"
  if [ "$3" != "-" ]; then echo "$3  $2" | shasum -a 256 -c -; fi
}
check() { echo "$2  $1" | shasum -a 256 -c -; }

# --- article/ (API responses can be re-rendered; hashes recorded for reference)
get "https://www.ebi.ac.uk/europepmc/webservices/rest/search?query=EXT_ID:31176169%20AND%20SRC:MED&format=json&resultType=core" \
    article/europepmc_31176169.json -   # 819274eb2ff1d7c338851797beee869b4a981f9b288ec564750fb6ba0cbac205
get "https://data.mendeley.com/public-api/datasets/m34zcvp3nd" article/mendeley_m34zcvp3nd.json -   # c0ea30e6d6675c16825f7f24fd7607d99f58abceb5a145402821bef25006120c
get "https://data.mendeley.com/public-api/datasets/m34zcvp3nd/files?folder_id=root&version=1" \
    article/mendeley_m34zcvp3nd_files.json -   # 437a3715bc9ea67c72c45a5014d589745359345cd65d077150224295b41dca4f
ELS="https://ars.els-cdn.com/content/image/1-s2.0-S075333221931529X"
get "$ELS-gr1.jpg"     article/gr1.jpg     a6cdfe142f1becbd0d157f1b7cdf1bcedd4ece68ce848f5029f8002a4bf0faa2
get "$ELS-gr1_lrg.jpg" article/gr1_lrg.jpg b2b9ec7d235b304f95768dd1491b774ab8060b8175754429398940883e210e65
get "$ELS-gr2.jpg"     article/gr2.jpg     421685eb33dc0c2185d0826d8a162d3782821a4d7e641c472e3ad7449948b370
get "$ELS-gr2_lrg.jpg" article/gr2_lrg.jpg ec51d9a8825a3199a702f344dc1958064fef98e2c06296d369be46ad84437def
get "$ELS-gr5.jpg"     article/gr5.jpg     9a3a33717a182f31c8b1e863fb0b212df7d239b720b96e813909205633b0921c
get "$ELS-gr5_lrg.jpg" article/gr5_lrg.jpg 20c91e842ee339a3c42bc5f325b9faf41921b05784feb6a9b18b7ca2fb36a2df

# --- authors/ (bar charts of the western blot quantification)
get "$ELS-gr3.jpg"     authors/gr3.jpg     1b944736202a3b7e40480f2b68f4ea999c037562946cb2f8e32cd7de090b18f6
get "$ELS-gr3_lrg.jpg" authors/gr3_lrg.jpg 712b12212accc64e24a10147ade5c9162c80e9b60b7a4bc6493a9fd6cf255a26
get "$ELS-gr4.jpg"     authors/gr4.jpg     f68fd7069b1eba08e5c526c3dfe45f1647e16842878a4d2eab36f7492eb7eee1
get "$ELS-gr4_lrg.jpg" authors/gr4_lrg.jpg 89e3a90206271b07589e2f6d0ec281d6e7b952851a09084bd14b45f8575bef29
get "$ELS-gr6.jpg"     authors/gr6.jpg     4c13483af8c729f985e7849db67cec5c81b6bc86e31b7f976af7786a1949fbeb
get "$ELS-gr6_lrg.jpg" authors/gr6_lrg.jpg 86f99541506fc87db6cfe1d0ac2b25b1283de1daada77a2eceefd715e8d73edc
get "$ELS-gr7.jpg"     authors/gr7.jpg     a890422f14a5d2a9db86fd278bf78b97b69f6060d37daefa0752bb84c1f95e6c
get "$ELS-gr7_lrg.jpg" authors/gr7_lrg.jpg 9c406260bde7df5066770df0103cd1b5b7c5adfaca0ac17d4555446e1dbba5db

# --- article/fig3G_lane_labels.jpg: Fig. 3G only (blots and lane labels, no bar chart), cropped
#     from authors/gr3_lrg.jpg (2934 x 3413): 1520 x 600 px at x = 0, y = 2700. Needs macOS sips.
sips -c 600 1520 --cropOffset 2700 0 authors/gr3_lrg.jpg --out article/fig3G_lane_labels.jpg >/dev/null
check article/fig3G_lane_labels.jpg d3b05cbf7511b67708cfc2aee3242efadaa3eb66758ba453334f6a19a194faef

# --- Mendeley Data m34zcvp3nd v1 (10.17632/m34zcvp3nd.1), "supplementary figures.zip"
get "https://data.mendeley.com/public-files/datasets/m34zcvp3nd/files/058ee801-d911-42d6-ae42-77604d29ea40/file_downloaded" \
    "archives/supplementary figures.zip" b22663d2b35f0d905bdd4ea33d6560bf39da6c65eb9736bd20b52c6bec957beb
# Extract only the Fig. 3G membranes (two other members have non-UTF-8 names and are not needed).
unzip -o -q -j "archives/supplementary figures.zip" "supplementary figures/MHC.tif" \
    "supplementary figures/BAF8.tif" "supplementary figures/F59.tif" \
    "supplementary figures/Gapdh(fig3).tif" -d images
check images/MHC.tif          ab2f04a9a1895ba7e4b567607da08a9f8a678412f9d6d9a73e6001fdcbd311e3
check images/BAF8.tif         3de8822dd910c4008ccc96fc758be67a91eb574550b40d749d9bfb93114628f9
check images/F59.tif          a270a0f224490ef1e827af1f3d6c9ab7652d80a53ecb2140ca5537255847b250
check "images/Gapdh(fig3).tif" 50576caddb05992fe34ded9d09bb67c95bb201aa52daef5ecc70ac2fc3241dd6
