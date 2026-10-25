import QtQuick
import BlotVerse
import QtQuick.Layouts

// The analysis stages, ticking off as they complete.
ColumnLayout {
    id: root
    property int current: 0
    property real fraction: 0
    property var stages: [qsTr("Quality control & lanes"), qsTr("Quantification (branch cache)"),
                          qsTr("Universes"), qsTr("Joint permutation test"), qsTr("Summary")]
    spacing: 14

    Repeater {
        model: root.stages
        delegate: RowLayout {
            required property int index
            required property string modelData
            readonly property bool done: index < root.current || (index === root.current && root.fraction >= 1 && index === 4)
            readonly property bool active: index === root.current && !done
            spacing: 12
            opacity: index <= root.current ? 1 : 0.4
            Behavior on opacity { NumberAnimation { duration: Theme.normal } }

            Item {
                width: 22; height: 22
                Rectangle {
                    anchors.fill: parent; radius: 11
                    color: done ? Theme.robust : "transparent"
                    border.width: done ? 0 : 2
                    border.color: active ? Theme.accent : Theme.faint
                    Behavior on color { ColorAnimation { duration: Theme.normal } }
                }
                Text { anchors.centerIn: parent; text: "✓"; color: "#0b1220"; font.pixelSize: 13; font.bold: true; visible: done }
                Rectangle {   // spinner dot
                    visible: active
                    width: 6; height: 6; radius: 3; color: Theme.accent
                    x: 11 + 7 * Math.cos(spin.a) - 3; y: 11 + 7 * Math.sin(spin.a) - 3
                    QtObject { id: spin; property real a: 0; NumberAnimation on a { running: active; from: 0; to: Math.PI * 2; duration: 900; loops: Animation.Infinite } }
                }
            }
            Text { text: modelData; color: active ? Theme.text : Theme.muted; font.pixelSize: 15; Layout.fillWidth: true }
            Text {
                visible: active && root.fraction > 0 && root.fraction < 1
                text: Math.round(root.fraction * 100) + " %"
                color: Theme.accent; font.pixelSize: 13; font.features: { "tnum": 1 }
            }
        }
    }
}
