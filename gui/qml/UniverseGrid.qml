import QtQuick
import BlotVerse

// One dot per universe. Dots light up as universes are evaluated; during the
// joint permutation test a wave runs across them.
Item {
    id: root
    property int count: 432
    property real litFraction: 0     // universes evaluated so far
    property bool waving: false      // joint test running
    property real t: 0
    NumberAnimation on t { running: root.waving; from: 0; to: Math.PI * 2; duration: 1600; loops: Animation.Infinite }

    readonly property int columns: Math.max(1, Math.round(Math.sqrt(count * width / Math.max(1, height))))
    readonly property real cell: Math.min(width / columns, height / Math.ceil(count / columns))

    Repeater {
        model: root.count
        delegate: Rectangle {
            required property int index
            readonly property bool lit: index < Math.round(root.litFraction * root.count)
            readonly property int col: index % root.columns
            readonly property int row: Math.floor(index / root.columns)
            x: col * root.cell + (root.cell - width) / 2
            y: row * root.cell + (root.cell - height) / 2
            width: root.cell * 0.62; height: width; radius: width / 2
            color: lit ? (col % 2 ? Theme.accent : Theme.accent2) : "#10ffffff"
            opacity: root.waving ? 0.45 + 0.55 * Math.abs(Math.sin(root.t + (col + row) * 0.22)) : 1
            scale: lit ? 1 : 0.7
            Behavior on color { ColorAnimation { duration: 250 } }
            Behavior on scale { NumberAnimation { duration: 300; easing.type: Easing.OutBack } }
        }
    }
}
