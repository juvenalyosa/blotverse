import QtQuick
import BlotVerse
import QtQuick.Controls.Basic

// Specification curve: every valid universe ordered by effect (never by p),
// with the branch panel underneath. Draws itself in from left to right.
Item {
    id: root
    property var curve: []     // [{lfc, p, kind, isDefault, label}]
    property var panel: []     // [{decision, branches: [{id, ranks}]}]
    property real reveal: 0
    property int hover: -1

    readonly property real plotLeft: 150
    readonly property real plotRight: 12
    readonly property real curveHeight: height * 0.46
    readonly property real rowHeight: 12
    readonly property real dx: (width - plotLeft - plotRight) / Math.max(1, curve.length)

    onCurveChanged: { reveal = 0; revealAnim.restart() }
    NumberAnimation { id: revealAnim; target: root; property: "reveal"; from: 0; to: 1; duration: 1400; easing.type: Easing.OutCubic }
    onRevealChanged: canvas.requestPaint()
    onHoverChanged: canvas.requestPaint()
    onWidthChanged: canvas.requestPaint()
    onHeightChanged: canvas.requestPaint()

    function range() {
        let lo = 0, hi = 0
        for (const p of curve) { lo = Math.min(lo, p.lfc); hi = Math.max(hi, p.lfc) }
        const pad = 0.08 * Math.max(1e-6, hi - lo)
        return [lo - pad, hi + pad]
    }

    Canvas {
        id: canvas
        anchors.fill: parent
                onPaint: {
            const ctx = getContext("2d")
            ctx.reset()
            const n = root.curve.length
            if (n === 0) return
            const [lo, hi] = root.range()
            const top = 8, h = root.curveHeight - 16
            const y = v => top + h * (hi - v) / (hi - lo)
            const shown = Math.ceil(n * root.reveal)

            // Axis, zero line and ticks.
            ctx.font = "11px sans-serif"
            ctx.fillStyle = String(Theme.muted)
            ctx.strokeStyle = "rgba(255,255,255,0.13)"
            ctx.lineWidth = 1
            for (let t = 0; t <= 4; ++t) {
                const v = lo + (hi - lo) * t / 4
                ctx.fillText(v.toFixed(2), root.plotLeft - 44, y(v) + 4)
            }
            ctx.setLineDash([4, 4])
            ctx.beginPath(); ctx.moveTo(root.plotLeft, y(0)); ctx.lineTo(root.width - root.plotRight, y(0)); ctx.stroke()
            ctx.setLineDash([])
            ctx.save(); ctx.translate(14, top + h / 2); ctx.rotate(-Math.PI / 2)
            ctx.fillText(qsTr("log2 fold change"), -44, 0); ctx.restore()

            // Points.
            const colours = [String(Theme.notSignificant), String(Theme.positive), String(Theme.negative)]
            for (let i = 0; i < shown; ++i) {
                const p = root.curve[i]
                const x = root.plotLeft + (i + 0.5) * root.dx
                ctx.fillStyle = colours[p.kind]
                ctx.beginPath(); ctx.arc(x, y(p.lfc), i === root.hover ? 4.5 : 2.4, 0, 2 * Math.PI); ctx.fill()
            }
            // Default universe (pre-registered) is outlined.
            for (let i = 0; i < shown; ++i)
                if (root.curve[i].isDefault) {
                    const x = root.plotLeft + (i + 0.5) * root.dx
                    ctx.strokeStyle = String(Theme.text); ctx.lineWidth = 2
                    ctx.beginPath(); ctx.arc(x, y(root.curve[i].lfc), 6, 0, 2 * Math.PI); ctx.stroke()
                }

            // Branch panel.
            let row = root.curveHeight + 14
            for (const d of root.panel) {
                ctx.fillStyle = String(Theme.text); ctx.font = "bold 11px sans-serif"
                ctx.fillText(d.decision, 4, row + 9)
                row += root.rowHeight
                ctx.font = "11px sans-serif"
                for (const b of d.branches) {
                    ctx.fillStyle = String(Theme.muted)
                    ctx.fillText(b.id, root.plotLeft - 44, row + 9)
                    ctx.fillStyle = "rgba(141,153,175,0.55)"
                    for (const r of b.ranks)
                        if (r < shown) ctx.fillRect(root.plotLeft + r * root.dx, row + 2, Math.max(1, root.dx * 0.9), 7)
                    row += root.rowHeight
                }
            }
            if (root.hover >= 0) {
                ctx.strokeStyle = "rgba(255,255,255,0.2)"; ctx.lineWidth = 1
                const x = root.plotLeft + (root.hover + 0.5) * root.dx
                ctx.beginPath(); ctx.moveTo(x, top); ctx.lineTo(x, row); ctx.stroke()
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: mouse => {
            const i = Math.floor((mouse.x - root.plotLeft) / root.dx)
            root.hover = (i >= 0 && i < root.curve.length && mouse.y < root.curveHeight + 4) ? i : -1
        }
        onExited: root.hover = -1
    }

    ToolTip {
        id: tip
        visible: root.hover >= 0
        x: Math.min(root.width - width, root.plotLeft + (root.hover + 1) * root.dx + 8)
        y: 8
        delay: 0
        text: tipText(root.hover)
        function tipText(i) {
            if (i < 0 || i >= root.curve.length) return ""
            const p = root.curve[i]
            return p.label + "\n" + qsTr("lfc %1 · p %2").arg(Theme.fmt(p.lfc, 3)).arg(Theme.fmtP(p.p)) +
                   (p.isDefault ? "\n" + qsTr("pre-registered default") : "")
        }
        contentItem: Text { text: tip.text; color: Theme.text; font.pixelSize: 12; font.family: "Menlo" }
        background: Rectangle { color: Theme.cardSolid; radius: 8; border.color: Theme.border }
    }
}
