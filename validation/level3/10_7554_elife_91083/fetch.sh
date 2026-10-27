#!/usr/bin/env bash
# Re-downloads every file used for 10.7554/eLife.91083 (Fig. 6B/6D, LRRK2 pS935 / total LRRK2).
# article/  article text and legends, Fig. 6-figure supplement 2 (raw-blot overview with lane
#           labels), the deposit's lane annotation, Zenodo record metadata
# images/   raw Stella chemiluminescence images (16-bit) and white-light marker photos
# authors/  every file with the authors' numbers (annotators never open this folder):
#           article PDF and figures PDF (Fig. 6C/D bar charts), Fig. 6 image (bar charts),
#           Supplementary file 1 (table 1c: mean +/- error of pRab10 and pS935 per variant)
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p article images/Figure6_source_data_1/{A,B,C} authors
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT

get() { # url dest sha256 (sha "-" = page served dynamically, hash not checked)
  curl -sSL -o "$2" "$1"
  if [ "$3" != "-" ]; then echo "$3  $2" | shasum -a 256 -c -; fi
}
check() { echo "$2  $1" | shasum -a 256 -c -; }

# --- article (Europe PMC XML and the Zenodo API record can be re-rendered; hashes recorded for reference)
get "https://www.ebi.ac.uk/europepmc/webservices/rest/PMC11658767/fullTextXML" \
    article/elife-91083-PMC11658767.xml -   # 00705cc400926b1e18ab26e96663e00daa6f07fd426d8470447c98a8255ac592
get "https://zenodo.org/api/records/14055918" article/zenodo_14055918_record.json -   # d02f71f9a7d2f9c9fbcb40a379c00d10047379467f2f199bb3b85a374c5b9573
get "https://iiif.elifesciences.org/lax:91083%2Felife-91083-fig6-figsupp2-v1.tif/full/1500,/0/default.jpg" \
    article/elife-91083-fig6-figsupp2-v1.jpg 7f2a0d0d5d15634916c583f804f0ddf449dbc5fcf7729b2d3a0aef04430cd458

# --- authors' numbers -> authors/
get "https://cdn.elifesciences.org/articles/91083/elife-91083-v1.pdf" authors/elife-91083-v1.pdf \
    a573358c4496877814c35c949f971ac905b20614cae9d0c4c4dbec883da48da2
get "https://cdn.elifesciences.org/articles/91083/elife-91083-figures-v1.pdf" authors/elife-91083-figures-v1.pdf \
    cabf7514bda50e0530ebbdcd0eebb3a362815841986982d64ee7ad6a13c3737c
get "https://iiif.elifesciences.org/lax:91083%2Felife-91083-fig6-v1.tif/full/1500,/0/default.jpg" \
    authors/elife-91083-fig6-v1.jpg 9b7972df8c960ee6595a0424cab4f0351251e2f719e5f6febea410de1c96abf2
get "https://cdn.elifesciences.org/articles/91083/elife-91083-supp1-v1.docx" authors/elife-91083-supp1-v1.docx \
    8733d2e1bcef0408bfdc670b49b68abb66c4d2fcfdd3eca12ab24f9ecb74da7a

# --- Zenodo deposit 10.5281/zenodo.14055918 (concept 10.5281/zenodo.11242229), Figure6_source_data_1.zip
#     (record MD5 b92d5687b6a820c6682c1a6ebcdf3add)
get "https://zenodo.org/api/records/14055918/files/Figure6_source_data_1.zip/content" "$TMP/fig6.zip" \
    089cac3a2e7ed3cc1da3df87d6b13210bf3f596befe14f09bd6a362fe66774c9
unzip -o -q "$TMP/fig6.zip" Annotation.png -d "$TMP"
mv "$TMP/Annotation.png" article/Figure6_source_data_1_Annotation.png
check article/Figure6_source_data_1_Annotation.png 7ff88261925712352311681ab57ca5130b737c18afe11b741c8f7ef11ec61410
# Folders kept: A/focus.tif and B/focus.tif have the same name but differ.
# B/B_focus.tif is byte-identical to B/focus.tif and is not extracted.
unzip -o -q "$TMP/fig6.zip" "A/2023-02-07_2x2-300s-07.tif" "A/focus.tif" \
    "B/2023-02-17_2x2-180s-05.tif" "B/focus.tif" \
    "C/2023-02-20_24D8_2x2-180s-05.tif" "C/2023-02-20_24D8_focus.tif" -d images/Figure6_source_data_1
check images/Figure6_source_data_1/A/2023-02-07_2x2-300s-07.tif b6d2fba99a64b17d8d98d74e719ecedd55f9d0e5c7c3c96d24194628b45f0d01
check images/Figure6_source_data_1/A/focus.tif 57bfc16707a395cee29df1268a1e191e2582ffcca61d217b0ccb02c294585270
check images/Figure6_source_data_1/B/2023-02-17_2x2-180s-05.tif 5001716aab41fdde040f43172596432e94e0d1b60db172f191af9a8c35178f65
check images/Figure6_source_data_1/B/focus.tif 78d4db90ad2275a26e30e3a57aac59e01ba33c46900230339f54a4eee60063a8
check images/Figure6_source_data_1/C/2023-02-20_24D8_2x2-180s-05.tif 3d242ca400ae55c5aa81bad7896de5ad47a8e16954fedbccbc6c0526a082a4a8
check images/Figure6_source_data_1/C/2023-02-20_24D8_focus.tif b98cc80783b9d571cee27e6fd2c83d636fdf81f8a74d78c74731c55fce9dda43
