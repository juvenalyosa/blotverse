import QtQuick
import BlotVerse
import QtQuick.Controls.Basic

// A blot image with its lanes and band windows. Lanes can be dragged
// sideways and band handles up and down; changes go to the controller.
Item {
    id: root
    property int blotIndex: 0
    property var blot: null        // controller's blot map
    property int revision: 0

    readonly property real s: img.paintedWidth > 0 && blot ? img.paintedWidth / blot.width : 1
    readonly property real ox: (img.width - img.paintedWidth) / 2
    readonly property real oy: (img.height - img.paintedHeight) / 2

    Rectangle { anchors.fill: parent; radius: 14; color: "#f4f6fa" }

    Image {
        id: img
        anchors { fill: parent; margins: 10 }
        source: root.blot ? "image://blot/" + root.blotIndex + "/" + root.revision : ""
        fillMode: Image.PreserveAspectFit
        smooth: true
        cache: false
        opacity: status === Image.Ready ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: Theme.normal } }
    }

    Item {
        id: overlay
        x: img.x + root.ox; y: img.y + root.oy
        width: img.paintedWidth; height: img.paintedHeight
        visible: root.blot && root.blot.annotated

        Repeater {
            model: root.blot ? root.blot.lanes : []
            delegate: Item {
                id: laneItem
                required property int index
                required property var modelData
                readonly property color tone: Theme.groupColor(modelData.groupIndex)
                readonly property real hh: root.blot.halfHeight
                x: (modelData.x - modelData.width / 2) * root.s
                width: modelData.width * root.s
                height: overlay.height

                Rectangle {   // lane frame
                    anchors.fill: parent
                    color: laneDrag.containsMouse || laneDrag.drag.active ? Qt.rgba(tone.r, tone.g, tone.b, 0.12) : "transparent"
                    border.color: tone; border.width: 1.5; radius: 3
                    opacity: 0.85
                }
                Text {
                    anchors { horizontalCenter: parent.horizontalCenter; top: parent.top; topMargin: 4 }
                    text: modelData.group; color: tone; font.pixelSize: 11; font.weight: Font.DemiBold
                }
                MouseArea {
                    id: laneDrag
                    anchors.fill: parent
                    hoverEnabled: true
                    cursorShape: Qt.SizeHorCursor
                    drag.target: laneItem; drag.axis: Drag.XAxis
                    onReleased: app.moveLane(root.blotIndex, laneItem.index, (laneItem.x + laneItem.width / 2) / root.s)
                }

                // Band windows with draggable handles: target (solid) and reference (dashed).
                Repeater {
                    model: [{ target: true, y: modelData.targetY }, { target: false, y: modelData.referenceY }]
                    delegate: Rectangle {
                        id: band
                        required property var modelData
                        x: -2; width: laneItem.width + 4
                        y: (modelData.y - laneItem.hh) * root.s
                        height: (2 * laneItem.hh + 1) * root.s
                        color: Qt.rgba(laneItem.tone.r, laneItem.tone.g, laneItem.tone.b, bandDrag.containsMouse ? 0.32 : 0.18)
                        border.color: laneItem.tone; border.width: modelData.target ? 2 : 1
                        radius: 2
                        MouseArea {
                            id: bandDrag
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.SizeVerCursor
                            drag.target: band; drag.axis: Drag.YAxis
                            onReleased: app.moveBand(root.blotIndex, laneItem.index, band.modelData.target,
                                                     (band.y + band.height / 2) / root.s)
                        }
                    }
                }
            }
        }
    }

    Text {
        anchors.centerIn: parent
        visible: root.blot && !root.blot.annotated
        text: qsTr("Lanes could not be detected automatically.\nAdd lanes_x or band hints to the experiment file.")
        color: "#333"; horizontalAlignment: Text.AlignHCenter
    }
}
