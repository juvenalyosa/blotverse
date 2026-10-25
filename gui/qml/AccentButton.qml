import QtQuick
import BlotVerse
import QtQuick.Controls.Basic

// Gradient pill button with hover lift and press feedback.
Button {
    id: control
    property bool subtle: false
    property bool large: false
    implicitHeight: large ? 54 : 42
    implicitWidth: Math.max(large ? 220 : 120, label.implicitWidth + 44)
    hoverEnabled: true
    font.pixelSize: large ? 17 : 14
    font.weight: Font.DemiBold

    scale: down ? 0.97 : (hovered ? 1.03 : 1.0)
    Behavior on scale { NumberAnimation { duration: Theme.fast; easing.type: Easing.OutCubic } }

    contentItem: Text {
        id: label
        text: control.text
        font: control.font
        color: control.subtle ? Theme.text : "#0b1220"
        opacity: control.enabled ? 1 : 0.45
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }
    // Two plates cross-fade: a translucent one for subtle buttons and a
    // gradient one for primary actions.
    background: Item {
        opacity: control.enabled ? 1 : 0.4
        Rectangle {
            anchors.fill: parent
            radius: height / 2
            visible: control.subtle
            color: control.hovered ? "#14ffffff" : "#08ffffff"
            border.width: 1
            border.color: Theme.border
        }
        Rectangle {
            anchors.fill: parent
            radius: height / 2
            visible: !control.subtle
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: control.hovered ? Qt.lighter(Theme.accent, 1.12) : Theme.accent }
                GradientStop { position: 1.0; color: control.hovered ? Qt.lighter(Theme.accent2, 1.12) : Theme.accent2 }
            }
        }
    }
}
