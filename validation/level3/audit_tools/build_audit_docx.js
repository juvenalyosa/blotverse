// Builds the human-audit package (Word) for the level-3 auditor from the overlay PNGs.
const fs = require("fs");
const path = require("path");
const {
  Document, Packer, Paragraph, TextRun, ImageRun, Table, TableRow, TableCell, WidthType,
  ShadingType, AlignmentType, HeadingLevel, PageOrientation, PageBreak, LevelFormat, BorderStyle,
} = require("docx");

const PKG = process.argv[2];
const OUT = process.argv[3];
const CONTENT_W = 14400; // landscape Letter, 0.5" margins (DXA)
const MAX_PX = 900;

function pngSize(file) {
  const b = fs.readFileSync(file);
  return { w: b.readUInt32BE(16), h: b.readUInt32BE(20) };
}
function img(file, maxW = MAX_PX, maxH = 330) {
  const { w, h } = pngSize(file);
  const k = Math.min(maxW / w, maxH / h, 1);
  return new Paragraph({ alignment: AlignmentType.CENTER, spacing: { after: 120 }, children: [
    new ImageRun({ type: "png", data: fs.readFileSync(file),
      transformation: { width: Math.round(w * k), height: Math.round(h * k) } })] });
}
const p = (text, opts = {}) => new Paragraph({ spacing: { after: 120 }, ...opts,
  children: (Array.isArray(text) ? text : [text]).map(t => typeof t === "string" ? new TextRun(t) : t) });
const b = t => new TextRun({ text: t, bold: true });
const bullet = (text) => new Paragraph({ numbering: { reference: "bullets", level: 0 }, spacing: { after: 80 },
  children: (Array.isArray(text) ? text : [text]).map(t => typeof t === "string" ? new TextRun(t) : t) });
const h1 = t => new Paragraph({ heading: HeadingLevel.HEADING_1, children: [new TextRun(t)] });
const h2 = t => new Paragraph({ heading: HeadingLevel.HEADING_2, children: [new TextRun(t)] });
const brk = () => new Paragraph({ children: [new PageBreak()] });

const border = { style: BorderStyle.SINGLE, size: 4, color: "808080" };
const borders = { top: border, bottom: border, left: border, right: border };
function cell(text, w, { head = false, fill } = {}) {
  return new TableCell({ width: { size: w, type: WidthType.DXA }, borders,
    shading: fill ? { fill, type: ShadingType.CLEAR, color: "auto" } : undefined,
    margins: { top: 60, bottom: 60, left: 80, right: 80 },
    children: [new Paragraph({ alignment: head ? AlignmentType.CENTER : AlignmentType.LEFT,
      children: [new TextRun({ text, bold: head, size: head ? 18 : 20 })] })] });
}
function table(widths, rows, headFill = "D9E2F3") {
  return new Table({ width: { size: widths.reduce((a, c) => a + c, 0), type: WidthType.DXA }, columnWidths: widths,
    rows: rows.map((r, i) => new TableRow({ tableHeader: i === 0,
      children: r.map((t, j) => cell(t, widths[j], i === 0 ? { head: true, fill: headFill } : {})) })) });
}

// One row per lane; the auditor writes X in the boxes
function laneTable(lanes, targetName, refName) {
  const W = [700, 2300, 1100, 1100, 1150, 1150, 1150, 1150, 4600];
  const head = ["Nº", "Grupo asignado", "Grupo CORRECTO (X)", "Grupo INCORRECTO (X)",
    `${targetName}: bien encuadrada (X)`, `${targetName}: error menor (X)`,
    `${refName}: bien encuadrado (X)`, `${refName}: error menor (X)`, "Comentario (opcional)"];
  return table(W, [head, ...lanes.map((g, i) => [String(i + 1), g, "", "", "", "", "", "", ""])]);
}
function yesNo(rows) { // [question] -> table with Sí / No / Comentario
  const W = [7000, 900, 900, 5600];
  return table(W, [["Pregunta", "Sí (X)", "No (X)", "Comentario"], ...rows.map(q => [q, "", "", ""])]);
}
function choice(options) { // [option] -> table with X column
  const W = [900, 13500];
  return table(W, [["Su elección (X)", "Opción"], ...options.map(o => ["", o])]);
}

const exps = [
  {
    dir: "10_1038_s41467_026_71753_w",
    title: "Experimento 1 — Nature Communications 2026 (Volk et al.), Fig. 1e",
    what: [
      bullet([b("Qué se mide: "), "γH2AX en hígado, normalizado a la tinción de proteína total (Revert) de la misma membrana."]),
      bullet([b("Grupos: "), "Juvenil NT (control), Juvenil NDMA, Adulto NT, Adulto NDMA; 6 ratones por grupo (M1 M2 M3 F1 F2 F3)."]),
      bullet([b("Orden de carriles según los autores (Fig. Supl. 11b, abajo): "), "Juvenil NT | Juvenil NDMA | Adulto NT | Adulto NDMA, cada bloque de 6 separado por un marcador de peso molecular."]),
      bullet([b("Imagen A: "), "γH2AX (canal 800 nm, tira inferior de la membrana \"Blot 66\")."]),
      bullet([b("Imagen B: "), "tinción de proteína total de la misma membrana. Aquí el recuadro es alto a propósito: se mide todo el carril, no una banda."]),
      bullet([b("Nota de los anotadores: "), "en los bloques NT casi no se ve banda de γH2AX; la altura del recuadro se fijó a la de la banda en los carriles donde sí se ve."]),
    ],
    target: "γH2AX", ref: "Proteína total",
    extra: [],
  },
  {
    dir: "10_1016_j_molcel_2017_04_024",
    title: "Experimento 2 — Molecular Cell 2017 (Tsuchiya et al.), Fig. S7D",
    what: [
      bullet([b("Qué se mide: "), "Cdc48 en lisado total de levadura, normalizado a Pgk1."]),
      bullet([b("Grupos: "), "WT (control) y cdc48-3; 3 experimentos independientes."]),
      bullet([b("Lo que muestra el artículo (abajo): "), "solo un par de carriles recortado: WT a la izquierda, cdc48-3 a la derecha."]),
      bullet([b("Lo que hay en la membrana depositada: "), "6 carriles en 3 pares separados por un carril vacío. Los anotadores interpretaron cada par como un experimento, con WT a la izquierda como en la figura publicada."]),
      bullet([b("Imagen A: "), "Cdc48. Imagen B: Pgk1 (imagen aparte de la misma membrana)."]),
      bullet([b("Nota: "), "estas bandas están saturadas y el programa ya las descarta por eso; aun así hay que revisar si los carriles están bien asignados."]),
    ],
    target: "Cdc48", ref: "Pgk1",
    extra: [yesNo(["¿Está de acuerdo en que cada par de carriles es un experimento independiente, con WT a la izquierda y cdc48-3 a la derecha?"])],
  },
  {
    dir: "10_1016_j_celrep_2017_09_051",
    title: "Experimento 3 — Cell Reports 2017 (Charles et al.), Fig. Supl. 3C",
    what: [
      bullet([b("Qué se mide: "), "Fabp5 en lisado celular (CL) de explantes de tejido adiposo de ratones macho, normalizado a Gapdh."]),
      bullet([b("Grupos: "), "AL (ad libitum, control) y CR (restricción calórica); 6 ratones por grupo."]),
      bullet([b("Orden según los autores (Fig. Supl. 3A, panel superior, abajo, y la presentación que depositaron): "), "carriles 1–6 AL, carriles 7–12 CR."]),
      bullet([b("Imagen A: "), "Fabp5 CL (tira del medio de la membrana \"0701\")."]),
      bullet([b("Imagen B: "), "Gapdh (banda inferior de otra membrana, \"0619\", cargada con los mismos 12 lisados en el mismo orden)."]),
    ],
    target: "Fabp5", ref: "Gapdh",
    extra: [],
  },
];

const children = [];
// ---------- Cover and instructions
children.push(new Paragraph({ heading: HeadingLevel.TITLE, children: [new TextRun("Auditoría humana — BlotVerse, nivel 3")] }));
children.push(p([b("Para: "), "Dr. Jorge Leyva.   ", b("Preparado: "), "29 de octubre de 2026 (versión del repositorio: rama level3-protocol, commit 26c197d)."]));
children.push(h1("Qué le pedimos"));
children.push(p("Usted no necesita instalar nada ni usar ningún programa: todo lo de computación ya está hecho. Solo tiene que mirar imágenes y marcar con una X en este mismo documento."));
children.push(bullet([b("Parte 1 (tres experimentos): "), "dos programas de IA (anotadores A y B) marcaron, en las imágenes originales de los autores, qué carril pertenece a qué grupo y dónde está cada banda. Usted revisa si lo hicieron bien."]));
children.push(bullet([b("Parte 2 (tres decisiones): "), "casos donde el protocolo necesita el criterio de un experto."]));
children.push(bullet([b("Al terminar: "), "guarde el documento y envíeselo a Juvenal. Si algo no se entiende, escriba la duda en la columna de comentarios; no hace falta resolverla."]));
children.push(h1("Tres reglas"));
children.push(bullet([b("No use los valores numéricos de los autores "), "(gráficos de barras, tablas, material suplementario con cifras). Decida solo con la figura, la leyenda y los rótulos de los carriles."]));
children.push(bullet([b("No decida qué carril es de qué grupo por lo intensa que se ve la banda "), "(por ejemplo, \"este carril tiene más señal, así que debe ser el tratado\"). Solo por posición y rótulos."]));
children.push(bullet([b("Si no está seguro, "), "no marque nada en esa fila y escriba \"no sé\" y el motivo en el comentario."]));
children.push(h1("Cómo leer las imágenes"));
children.push(bullet("Cada recuadro de color es la zona que mide el programa en un carril. El número encima es el número de carril; la línea y el nombre debajo indican el grupo que le asignaron."));
children.push(bullet("Las imágenes se muestran con bandas oscuras sobre fondo claro y con el contraste aumentado solo para verlas mejor; los datos originales no se modifican."));
children.push(bullet([b("Imagen A "), "= la proteína que se estudia (\"diana\"). ", b("Imagen B "), "= el control de carga (la referencia para normalizar)."]));
children.push(h1("Cómo calificar cada carril (criterio fijado antes de empezar)"));
children.push(table([3000, 11400], [
  ["Calificación", "Cuándo usarla"],
  ["Grupo CORRECTO", "El carril pertenece al grupo que dice la etiqueta, según la figura y la leyenda del artículo."],
  ["Grupo INCORRECTO", "El carril es de otro grupo; o es un marcador, un carril vacío u otra muestra que no debía incluirse; o falta un carril que sí debía estar (anótelo en el comentario)."],
  ["Bien encuadrada", "El recuadro contiene la banda correcta y no toca la banda del carril vecino."],
  ["Error menor", "El carril es el correcto, pero el recuadro corta buena parte de la banda, toca la del vecino o está sobre otra banda (otra altura)."],
]));
children.push(p(""));
children.push(yesNo(["¿Acepta este criterio para calificar? (Si no, escriba el suyo en el comentario antes de empezar.)"]));

// ---------- Part 1
for (const e of exps) {
  const pk = path.join(PKG, e.dir);
  const exp = fs.readFileSync(path.join(PKG, e.dir, "lanes.txt"), "utf8").trim().split("\n");
  children.push(brk());
  children.push(h1("Parte 1. " + e.title));
  children.push(...e.what);
  children.push(h2("Figura publicada por los autores"));
  children.push(img(path.join(pk, "article_panel.png"), 900, 300));
  children.push(h2(`Imagen A — ${e.target} (con los recuadros de los anotadores)`));
  children.push(img(path.join(pk, "blot1_target_zoom.png")));
  children.push(h2(`Imagen B — ${e.ref} (control de carga)`));
  children.push(img(path.join(pk, "blot1_reference_zoom.png")));
  children.push(h2("Preguntas generales"));
  children.push(yesNo([
    `¿La imagen A corresponde a la ${e.target} del panel publicado (misma membrana y misma banda)?`,
    `¿La imagen B corresponde al control de carga (${e.ref}) de esos mismos carriles?`,
    "¿Están incluidos todos los carriles del experimento, y ninguno de más (marcadores, carriles vacíos, otras muestras)?",
    `¿El grupo control es el correcto (${exp[0]})?`,
  ]));
  children.push(...e.extra.flatMap(t => [p(""), t]));
  children.push(h2("Calificación carril por carril"));
  children.push(laneTable(exp, e.target, e.ref));
}

// ---------- Part 2
children.push(brk());
children.push(h1("Parte 2. Decisiones"));

children.push(h2("Decisión 1 — eLife 91083 (Fig. 6): ¿uno o dos experimentos?"));
children.push(p("En la figura 6B hay un solo conjunto de blots, pero los autores cuantifican dos cosas distintas:"));
children.push(bullet([b("6C: "), "pRab10 relativo a Rab10 total."]));
children.push(bullet([b("6D: "), "LRRK2 fosforilado en S935 relativo a LRRK2 total."]));
children.push(p("6C no se puede usar: en la tercera réplica, la tira de pRab10 está corrida casi un carril respecto a sus rótulos, y no se puede saber con certeza qué carril es de qué grupo. 6D sí cumple todos los criterios (WT frente a R1441G, 3 réplicas)."));
children.push(p("El protocolo dice que se toma el primer panel cuantificado que cumpla los criterios. La pregunta es si 6C y 6D cuentan como dos experimentos distintos o como uno solo:"));
children.push(choice([
  "Son experimentos distintos (cada uno mide una proteína distinta con su propio control). Se incluye 6D.",
  "Son un solo experimento (Fig. 6B). Como su primer resultado (6C) no se puede asignar, el artículo queda excluido.",
]));
children.push(p("Motivo (una o dos frases):"));
children.push(p("______________________________________________________________________________________________"));

children.push(h2("Decisión 2 — Cell Reports 2017 (experimento 3): dos desviaciones de los anotadores"));
children.push(p("Las reglas decían que los anotadores solo podían usar el artículo y las imágenes. Hubo dos desviaciones:"));
children.push(bullet([b("Anotador B "), "leyó por error el comienzo de un archivo de notas del cribado. Ese texto contenía la afirmación de los autores (que la restricción calórica aumenta Fabp5), pero ningún valor numérico. Él mismo lo declaró en su registro."]));
children.push(bullet([b("Anotador A "), "comparó la figura publicada con la imagen original para decidir la orientación (cuál extremo es el carril 1)."]));
children.push(p("Los dos asignaron los carriles de forma idéntica (1–6 AL, 7–12 CR), que coincide con la presentación que depositaron los autores."));
children.push(choice([
  "Aceptables: se registran como desviaciones y el experimento se mantiene.",
  "No aceptables: hay que volver a anotar este experimento con anotadores nuevos.",
]));
children.push(p("Comentario:"));
children.push(p("______________________________________________________________________________________________"));

children.push(h2("Decisión 3 — Cell Reports 2024 (eIF5A): ¿cómo se clasifica?"));
children.push(p("Este experimento pasó el cribado, y los dos anotadores asignaron los carriles igual. Pero el programa no puede analizarlo: cada condición está en una membrana distinta, con su propio Ponceau, y los 12 carriles de cada membrana son puntos de una serie temporal, no réplicas. BlotVerse necesita carriles control y tratados en la misma membrana, y trata los carriles como réplicas."));
children.push(choice([
  "Incluido pero \"no analizable\": se informa como una limitación del programa (así está registrado ahora).",
  "Debió excluirse en el cribado porque su diseño no cumple los criterios de inclusión (indique cuál en el comentario).",
]));
children.push(p("Comentario:"));
children.push(p("______________________________________________________________________________________________"));

// ---------- Sign-off
children.push(brk());
children.push(h1("Cierre"));
children.push(yesNo(["Confirmo que no usé los valores numéricos de los autores (gráficos de barras, tablas) para esta auditoría."]));
children.push(p(""));
children.push(p("Nombre: Jorge Leyva, PhD"));
children.push(p("Fecha: ____________________"));
children.push(p("Tiempo aproximado que le tomó: ____________________"));
children.push(p("Habrá una segunda entrega igual a esta con los experimentos que faltan (Biomedicine & Pharmacotherapy 2019, eLife si se incluye, y quizá Cryobiology 2018)."));

const doc = new Document({
  styles: {
    default: { document: { run: { font: "Arial", size: 22 } } },
    paragraphStyles: [
      { id: "Title", name: "Title", basedOn: "Normal", run: { size: 40, bold: true, color: "1F3864" }, paragraph: { spacing: { after: 240 } } },
      { id: "Heading1", name: "Heading 1", basedOn: "Normal", next: "Normal", quickFormat: true,
        run: { size: 30, bold: true, color: "1F3864" }, paragraph: { spacing: { before: 240, after: 120 }, outlineLevel: 0 } },
      { id: "Heading2", name: "Heading 2", basedOn: "Normal", next: "Normal", quickFormat: true,
        run: { size: 24, bold: true, color: "2E5597" }, paragraph: { spacing: { before: 200, after: 100 }, outlineLevel: 1 } },
    ],
  },
  numbering: { config: [{ reference: "bullets", levels: [{ level: 0, format: LevelFormat.BULLET, text: "•", alignment: AlignmentType.LEFT,
    style: { paragraph: { indent: { left: 540, hanging: 270 } } } }] }] },
  sections: [{ properties: { page: { size: { width: 12240, height: 15840, orientation: PageOrientation.LANDSCAPE },
    margin: { top: 720, bottom: 720, left: 720, right: 720 } } }, children }],
});
Packer.toBuffer(doc).then(buf => { fs.writeFileSync(OUT, buf); console.log("wrote", OUT); });
