import QtQuick
import BlotVerse
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
    id: root

    FileDialog {
        id: openDialog
        title: qsTr("Open a BlotVerse experiment")
        nameFilters: [qsTr("Experiment files (*.yaml *.yml)")]
        onAccepted: app.openExperiment(selectedFile)
    }

    ColumnLayout {
        anchors.centerIn: parent
        width: Math.min(1040, parent.width)
        spacing: 36

        // Hero.
        ColumnLayout {
            Layout.fillWidth: true
            spacing: 12
            Text {
                Layout.fillWidth: true
                text: qsTr("Does your western blot survive\nevery defensible analysis?")
                color: Theme.text; font.pixelSize: 44; font.weight: Font.Bold; lineHeight: 1.05
                horizontalAlignment: Text.AlignHCenter
                opacity: 0; Component.onCompleted: opacity = 1
                Behavior on opacity { NumberAnimation { duration: 900 } }
            }
            Text {
                Layout.fillWidth: true
                text: qsTr("BlotVerse runs every combination of background subtraction, band definition, loading control, normalisation, scale and test, and tells you how robust the conclusion is. It never picks a best universe.")
                color: Theme.muted; font.pixelSize: 16; wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignHCenter
                Layout.leftMargin: 80; Layout.rightMargin: 80
            }
        }

        RowLayout {
            Layout.fillWidth: true
            spacing: 24

            // Open an experiment (click or drop a YAML file).
            GlassCard {
                id: openCard
                Layout.fillWidth: true
                Layout.preferredHeight: 310
                glow: drop.containsDrag ? 1 : 0
                Behavior on glow { NumberAnimation { duration: Theme.fast } }
                DropArea {
                    id: drop
                    anchors.fill: parent
                    keys: ["text/uri-list"]
                    onDropped: d => { if (d.hasUrls) app.openExperiment(d.urls[0]) }
                }
                ColumnLayout {
                    anchors { fill: parent; margins: 28 }
                    spacing: 12
                    Text { text: qsTr("Analyse your blots"); color: Theme.text; font.pixelSize: 22; font.weight: Font.DemiBold }
                    Text {
                        text: qsTr("Open an experiment file (YAML) that lists your raw images — 16-bit TIFF, Bio-Rad .scn, ImageQuant .gel or LI-COR TIFF — and the group of every lane. Or drop it here.")
                        color: Theme.muted; font.pixelSize: 14; wrapMode: Text.WordWrap; Layout.fillWidth: true
                    }
                    Item { Layout.fillHeight: true }
                    AccentButton { text: qsTr("Open experiment…"); large: true; onClicked: openDialog.open() }
                }
            }

            // Simulated demo with known truth.
            GlassCard {
                Layout.fillWidth: true
                Layout.preferredHeight: 310
                ColumnLayout {
                    anchors { fill: parent; margins: 28 }
                    spacing: 12
                    Text { text: qsTr("Try a simulated experiment"); color: Theme.text; font.pixelSize: 22; font.weight: Font.DemiBold }
                    Text {
                        text: qsTr("Three gels, six replicates per group, with known truth. Choose the artefact level and the true effect.")
                        color: Theme.muted; font.pixelSize: 14; wrapMode: Text.WordWrap; Layout.fillWidth: true
                    }
                    RowLayout {
                        spacing: 8
                        Repeater {
                            id: levels
                            property int selected: 1
                            model: [qsTr("clean"), qsTr("moderate"), qsTr("severe")]
                            delegate: AccentButton {
                                required property int index
                                required property string modelData
                                text: modelData
                                subtle: levels.selected !== index
                                onClicked: levels.selected = index
                            }
                        }
                    }
                    RowLayout {
                        spacing: 12
                        Text { text: qsTr("True log2 fold change"); color: Theme.muted; font.pixelSize: 13 }
                        Slider { id: lfc; from: 0; to: 1.5; stepSize: 0.1; value: 1.0; Layout.fillWidth: true }
                        Text { text: lfc.value.toFixed(1); color: Theme.text; font.pixelSize: 14; font.features: { "tnum": 1 } }
                    }
                    Item { Layout.fillHeight: true }
                    AccentButton {
                        text: qsTr("Simulate and set up"); large: true
                        onClicked: app.simulateDemo(["clean", "moderate", "severe"][levels.selected], lfc.value)
                    }
                }
            }
        }

        Text {
            Layout.alignment: Qt.AlignHCenter
            text: app.specSummary
            color: Theme.faint; font.pixelSize: 12
        }
    }
}
