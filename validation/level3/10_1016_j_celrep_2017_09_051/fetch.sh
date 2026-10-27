#!/usr/bin/env bash
# Re-downloads every file used for 10.1016/j.celrep.2017.09.051 (Supplementary Fig. 3C, Fabp5 / Gapdh,
# male adipose explant lysates, AL vs CR).
# article/  article text (PMC author manuscript), main figures, Supp. Fig. 3 legend, deposit overview (pptx)
# images/   raw 16-bit TIFFs of the two membranes (Fabp5 CL strip; Gapdh CL strip) and their white-light images
# authors/  every file with the authors' numbers (Supplemental PDF with the S3C bar chart; article+SI PDF)
# No email address or other personal identifier is sent to any service.
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p article images authors
TMP="$(mktemp -d)"; trap 'rm -rf "$TMP"' EXIT

get() { # url dest sha256 ("-" = page served dynamically, hash not checked)
  curl -sSL -o "$2" "$1"
  if [ "$3" != "-" ]; then echo "$3  $2" | shasum -a 256 -c -; fi
}

# --- article (PMC5682597; HTML can change when re-rendered, hash recorded for reference)
get "https://pmc.ncbi.nlm.nih.gov/articles/PMC5682597/" article/pmc.html -   # 269bf802b965d0e3faa2ebfc39b3eeb246993d92490fcc869ee115f70c116fa4
PMC=https://cdn.ncbi.nlm.nih.gov/pmc/blobs/3128/5682597
get "$PMC/766fe21d6161/nihms910522f1.jpg" article/nihms910522f1.jpg 83e8b151a1f79f37043067251939f905f664d10cf9ee58dbb1c948bddba1c589
get "$PMC/6f29475b0356/nihms910522f2.jpg" article/nihms910522f2.jpg 9ea8de94f254e00edf42a7df7505e097569e28005c01058f8fe31a76c089345a
get "$PMC/af31148b9cd3/nihms910522f3.jpg" article/nihms910522f3.jpg 633ea7ffbffe8fd9171a8e3e4e0b909ca56ea8f1ace37fe6861aeda45325f2f2
get "$PMC/f5cc0636c9b5/nihms910522f4.jpg" article/nihms910522f4.jpg ab7530dc13a337832afb8c800fd5ca00320c46e129699eeafefb372ac6bf73be

# --- Supplemental Information (Elsevier CDN). mmc1 prints the S3B/S3C bar charts -> authors/
get "https://ars.els-cdn.com/content/image/1-s2.0-S2211124717313414-mmc1.pdf" authors/S2211124717313414-mmc1.pdf \
    02a1d8173b19be9801bbb5f02eed667629edc04c17a6a05c92409a2453691ba5
get "https://ars.els-cdn.com/content/image/1-s2.0-S2211124717313414-mmc2.pdf" authors/S2211124717313414-mmc2.pdf \
    a4496518bfd893cc9f59ab3b0e37d898071c4156e28dac8e1eb786e2933bedd2

# Supp. Fig. 3 legend for annotators (text of mmc1 page 4 only, no values), extracted with macOS PDFKit.
# Derived file; the hash below is what this extractor produced (3024e8f0b178fd8d26d301c9b157bdb9dc90b2587eba23e2d29db2c12c2465d0).
cat > "$TMP/pdftext.swift" <<'EOF'
import PDFKit
let d = PDFDocument(url: URL(fileURLWithPath: CommandLine.arguments[1]))!
print(d.page(at: Int(CommandLine.arguments[2])! - 1)?.string ?? "")
EOF
swift "$TMP/pdftext.swift" authors/S2211124717313414-mmc1.pdf 4 > article/SupplementaryFigure3_legend.txt

# --- Mendeley Data rp8s5rd98p (v1)
MD=https://data.mendeley.com/public-files/datasets/rp8s5rd98p/files
# overview: lane/group labels and which strip is which (Fabp5 CL = capture 4, Gapdh CL = capture 5); no values
get "$MD/800bd8b7-f6e2-47f8-b756-e14c3037803e/file_downloaded" "article/Fig S3A Male_Data Preparation.pptx" \
    c4bdb0b31478e00e5bfab6bc3c1d6d4cad712cceaaa162a9f88d49b9909d0fb7
# target membrane: Gapdh (repeat, not used) / Fabp5 CL (middle strip) / Fabp5 CM (bottom strip)
get "$MD/c4a88ce3-cd24-4357-8045-698f7b8a5f9c/file_downloaded" "images/0701 fat23 Gapdh Fabp5 CL-CM.tif" \
    e8504bae78138c2edf9e8993bf9cecc057bd6e04d020779bd557f61e46a132f2
get "$MD/cce9cc83-491e-4d21-afd1-901f2978b01b/file_downloaded" "images/0701 fat23 Gapdh Fabp5 CL-CM w.tif" \
    f03e2ad5014e6d9add1fa00cedb6cbf11b14fbb44a472b7b2961af699112ad9e
# loading-control membrane: Fasn (top) / Acas2 (middle) / Gapdh (bottom strip, used in the figure)
get "$MD/2fec4c1f-e76e-4e98-878b-7b4a694f8862/file_downloaded" "images/0619 fat21 Fasn AceS CL.tif" \
    327ac9b52a6abc120e9b34753a83431d0094d9a82862df977d939a01375a9465
get "$MD/030373a8-94ef-4411-b137-b475b61427ee/file_downloaded" "images/0619 fat21 Fasn AceS CL w.tif" \
    1e71eb6a0db1ffcbaa4a72282655c9e0ae9231def29654b2670034014fa36cec
