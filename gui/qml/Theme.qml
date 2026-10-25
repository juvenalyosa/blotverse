pragma Singleton
import QtQuick

// Colours, type and motion shared by every screen (the HTML report uses the
// same palette, so the application and its reports look like one product).
QtObject {
    readonly property color bg0: "#0a0e16"
    readonly property color bg1: "#101726"
    readonly property color card: "#cc151d2d"
    readonly property color cardSolid: "#162033"
    readonly property color border: "#17ffffff"
    readonly property color text: "#e9eef6"
    readonly property color muted: "#8d99af"
    readonly property color faint: "#5b667c"

    readonly property color accent: "#5fb3d1"
    readonly property color accent2: "#8b7cf6"
    readonly property color positive: "#5fb3d1"
    readonly property color negative: "#e07b5f"
    readonly property color notSignificant: "#4a5468"

    readonly property color robust: "#4cc38a"
    readonly property color fragile: "#e8b04a"
    readonly property color unsupported: "#ef6b6b"
    readonly property color unevaluable: "#7d879b"

    readonly property var groupColors: ["#5fb3d1", "#e8b04a", "#8b7cf6", "#4cc38a", "#ef6b6b", "#f08bd0"]

    readonly property int radius: 18
    readonly property int gap: 16
    readonly property string font: Qt.platform.os === "osx" ? "SF Pro Display" : "Inter"

    readonly property int fast: 180
    readonly property int normal: 350
    readonly property int slow: 900

    function verdictColor(key) {
        // Verdict enum: 0 Robust, 1 Fragile, 2 Not supported, 3 Not evaluable.
        return [robust, fragile, unsupported, unevaluable][key] ?? unevaluable
    }
    function groupColor(i) { return groupColors[i % groupColors.length] }
    function fmt(v, digits) { return (v === undefined || v === null || isNaN(v)) ? "–" : Number(v).toFixed(digits) }
    function fmtP(p) {
        if (p === undefined || p === null || isNaN(p)) return "–"
        return p < 1e-4 ? Number(p).toExponential(1) : Number(p).toFixed(4)
    }
}
