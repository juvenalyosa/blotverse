import QtQuick
import BlotVerse
import QtQuick.Layouts

// Share of the effect's variance explained by each decision (eta squared).
ColumnLayout {
    id: root
    property var bars: []   // [{label, value, top}]
    property real grow: 0
    spacing: 10
    onBarsChanged: { grow = 0; growAnim.restart() }
    NumberAnimation { id: growAnim; target: root; property: "grow"; from: 0; to: 1; duration: 1100; easing.type: Easing.OutCubic }

    readonly property real maxValue: {
        let m = 0
        for (const b of bars) m = Math.max(m, b.value)
        return m > 0 ? m : 1
    }

    Repeater {
        model: root.bars
        delegate: ColumnLayout {
            required property var modelData
            Layout.fillWidth: true
            spacing: 4
            RowLayout {
                Layout.fillWidth: true
                Text { text: modelData.label; color: modelData.top ? Theme.text : Theme.muted; font.pixelSize: 13; Layout.fillWidth: true }
                Text { text: Theme.fmt(modelData.value * root.grow, 3); color: Theme.muted; font.pixelSize: 12; font.features: { "tnum": 1 } }
            }
            Rectangle {
                Layout.fillWidth: true
                height: 8; radius: 4; color: "#0dffffff"
                Rectangle {
                    height: parent.height; radius: 4
                    width: parent.width * modelData.value / root.maxValue * root.grow
                    gradient: Gradient {
                        orientation: Gradient.Horizontal
                        GradientStop { position: 0; color: modelData.top ? Theme.accent : Theme.notSignificant }
                        GradientStop { position: 1; color: modelData.top ? Theme.accent2 : Theme.faint }
                    }
                }
            }
        }
    }
}
