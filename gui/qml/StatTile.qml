import QtQuick
import BlotVerse
import QtQuick.Layouts

// A number that counts up when it appears, with a caption.
GlassCard {
    id: root
    property string title
    property real value: NaN
    property int digits: 3
    property string text: ""      // overrides the number when set
    property string caption: ""
    property color valueColor: Theme.text
    // Counts up from zero whenever a new value arrives.
    property real shown: isNaN(value) ? 0 : value
    Behavior on shown { NumberAnimation { duration: Theme.slow; easing.type: Easing.OutCubic } }
    implicitHeight: 112

    ColumnLayout {
        anchors { fill: parent; margins: 18 }
        spacing: 4
        Text { text: root.title; color: Theme.muted; font.pixelSize: 13 }
        Text {
            text: root.text !== "" ? root.text : (isNaN(root.value) ? "–" : root.shown.toFixed(root.digits))
            color: root.valueColor
            font.pixelSize: 30; font.weight: Font.DemiBold
            font.features: { "tnum": 1 }
            elide: Text.ElideRight
            Layout.fillWidth: true
        }
        Text { text: root.caption; color: Theme.faint; font.pixelSize: 12; elide: Text.ElideRight; Layout.fillWidth: true }
    }
}
