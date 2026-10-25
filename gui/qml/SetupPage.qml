import QtQuick
import BlotVerse
import QtQuick.Controls.Basic
import QtQuick.Layouts

Item {
    id: root
    property int current: 0
    readonly property var blot: app.blots.length > current ? app.blots[current] : null
    Connections { target: app; function onExperimentChanged() { if (root.current >= app.blots.length) root.current = 0 } }

    RowLayout {
        anchors.fill: parent
        spacing: 20

        // Blot with lanes and bands.
        GlassCard {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ColumnLayout {
                anchors { fill: parent; margins: 20 }
                spacing: 14
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    Repeater {
                        model: app.blots
                        delegate: AccentButton {
                            required property int index
                            required property var modelData
                            text: modelData.id
                            subtle: index !== root.current
                            onClicked: root.current = index
                        }
                    }
                    Item { Layout.fillWidth: true }
                    Text {
                        text: root.blot ? qsTr("%1 · %2 × %3 px · lanes: %4")
                                          .arg(root.blot.file).arg(root.blot.width).arg(root.blot.height)
                                          .arg(root.blot.annotationSource === "manual" ? qsTr("corrected") : qsTr("detected")) : ""
                        color: Theme.muted; font.pixelSize: 12
                    }
                }
                BlotView {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    blotIndex: root.current
                    blot: root.blot
                    revision: app.imageRevision
                }
                Text {
                    text: qsTr("Drag a lane sideways or a band window up and down to correct it. Solid window: target band; thin window: loading control.")
                    color: Theme.faint; font.pixelSize: 12; wrapMode: Text.WordWrap; Layout.fillWidth: true
                }
            }
        }

        // Experiment panel.
        GlassCard {
            Layout.preferredWidth: 360
            Layout.fillHeight: true
            ColumnLayout {
                anchors { fill: parent; margins: 22 }
                spacing: 14
                Text { text: app.experimentName; color: Theme.text; font.pixelSize: 20; font.weight: Font.DemiBold; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                Text {
                    text: qsTr("%1 blot(s) · %2 universes").arg(app.blots.length).arg(app.universeTotal)
                    color: Theme.muted; font.pixelSize: 13
                }

                Text { text: qsTr("Lanes"); color: Theme.muted; font.pixelSize: 12; Layout.topMargin: 6 }
                Flow {
                    Layout.fillWidth: true
                    spacing: 6
                    Repeater {
                        model: root.blot ? root.blot.lanes : []
                        delegate: Rectangle {
                            required property int index
                            required property var modelData
                            readonly property color tone: Theme.groupColor(modelData.groupIndex)
                            width: chip.implicitWidth + 18; height: 26; radius: 13
                            color: Qt.rgba(tone.r, tone.g, tone.b, 0.15); border.color: tone
                            Text { id: chip; anchors.centerIn: parent; text: (index + 1) + " · " + modelData.group; color: Theme.text; font.pixelSize: 12 }
                        }
                    }
                }

                RowLayout {
                    Layout.topMargin: 6
                    Text { text: qsTr("Band window half-height"); color: Theme.muted; font.pixelSize: 13; Layout.fillWidth: true }
                    AccentButton { text: "−"; subtle: true; implicitWidth: 42; onClicked: app.setBandHalfHeight(root.current, root.blot.halfHeight - 1) }
                    Text {
                        text: root.blot ? root.blot.halfHeight : ""
                        color: Theme.text; font.pixelSize: 16; font.features: { "tnum": 1 }
                        horizontalAlignment: Text.AlignHCenter; Layout.preferredWidth: 32
                    }
                    AccentButton { text: "+"; subtle: true; implicitWidth: 42; onClicked: app.setBandHalfHeight(root.current, root.blot.halfHeight + 1) }
                }

                ColumnLayout {
                    visible: root.blot && root.blot.warnings.length > 0
                    Layout.fillWidth: true
                    spacing: 4
                    Text { text: qsTr("Image checks"); color: Theme.fragile; font.pixelSize: 12 }
                    Repeater {
                        model: root.blot ? root.blot.warnings : []
                        delegate: Text { required property string modelData; text: "• " + modelData; color: Theme.muted; font.pixelSize: 12; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                    }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    spacing: 8
                    AccentButton { text: qsTr("Re-detect"); subtle: true; onClicked: app.redetect(root.current) }
                    AccentButton { text: qsTr("Save corrections"); subtle: true; onClicked: app.saveAnnotations() }
                }
                AccentButton {
                    Layout.fillWidth: true
                    text: qsTr("Run the multiverse")
                    large: true
                    onClicked: app.run()
                }
                AccentButton { Layout.fillWidth: true; text: qsTr("New analysis"); subtle: true; onClicked: app.reset() }
            }
        }
    }
}
