import QtQuick
import BlotVerse
import QtQuick.Effects

// Translucent card with a hairline border and a soft shadow.
Item {
    id: root
    default property alias content: inner.data
    property color tint: Theme.card
    property real glow: 0.0   // 0..1, accent-coloured halo (used while active)

    Rectangle {
        id: plate
        anchors.fill: parent
        radius: Theme.radius
        color: root.tint
        border.color: Theme.border
        border.width: 1
        layer.enabled: true
        layer.effect: MultiEffect {
            shadowEnabled: true
            shadowColor: root.glow > 0 ? Theme.accent : "#000000"
            shadowOpacity: 0.35 + 0.4 * root.glow
            shadowBlur: 0.8
            shadowVerticalOffset: root.glow > 0 ? 0 : 8
        }
    }
    Item { id: inner; anchors.fill: parent }
}
