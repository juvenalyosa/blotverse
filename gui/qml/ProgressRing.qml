import QtQuick
import BlotVerse
import QtQuick.Shapes
import QtQuick.Effects

// Circular progress with a gradient arc, a slowly orbiting highlight and a
// breathing glow while work is in progress.
Item {
    id: root
    property real value: 0          // 0..1
    property bool busy: true
    property real thickness: 14
    property real shown: value
    Behavior on shown { NumberAnimation { duration: 400; easing.type: Easing.OutCubic } }

    readonly property real r: Math.min(width, height) / 2 - thickness

    // Glow underneath.
    Rectangle {
        anchors.centerIn: parent
        width: root.r * 2 + root.thickness * 3; height: width; radius: width / 2
        color: "transparent"
        border.width: root.thickness * 1.6
        border.color: Theme.accent
        opacity: root.busy ? breathe.o : 0.15
        layer.enabled: true
        layer.effect: MultiEffect { blurEnabled: true; blur: 1.0; blurMax: 48 }
        QtObject {
            id: breathe
            property real o: 0.25
            SequentialAnimation on o {
                running: root.busy; loops: Animation.Infinite
                NumberAnimation { to: 0.45; duration: 1200; easing.type: Easing.InOutSine }
                NumberAnimation { to: 0.2; duration: 1200; easing.type: Easing.InOutSine }
            }
        }
    }

    Shape {
        anchors.fill: parent
        preferredRendererType: Shape.CurveRenderer
        // Track.
        ShapePath {
            strokeColor: "#12ffffff"; strokeWidth: root.thickness; fillColor: "transparent"
            capStyle: ShapePath.RoundCap
            PathAngleArc { centerX: root.width / 2; centerY: root.height / 2; radiusX: root.r; radiusY: root.r; startAngle: 0; sweepAngle: 360 }
        }
        // Progress arc.
        ShapePath {
            strokeWidth: root.thickness; fillColor: "transparent"; capStyle: ShapePath.RoundCap
            strokeColor: Theme.accent
            fillGradient: null
            PathAngleArc {
                centerX: root.width / 2; centerY: root.height / 2; radiusX: root.r; radiusY: root.r
                startAngle: -90; sweepAngle: Math.max(0.5, 360 * root.shown)
            }
        }
    }

    // Orbiting highlight at the head of the arc.
    Rectangle {
        width: root.thickness * 1.4; height: width; radius: width / 2
        color: Theme.accent2
        visible: root.busy
        readonly property real a: (-90 + 360 * root.shown) * Math.PI / 180
        x: root.width / 2 + root.r * Math.cos(a) - width / 2
        y: root.height / 2 + root.r * Math.sin(a) - height / 2
        layer.enabled: true
        layer.effect: MultiEffect { shadowEnabled: true; shadowColor: Theme.accent2; shadowBlur: 1.0; shadowOpacity: 0.9; shadowVerticalOffset: 0; shadowHorizontalOffset: 0 }
    }
}
