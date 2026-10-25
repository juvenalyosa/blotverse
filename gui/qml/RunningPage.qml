import QtQuick
import BlotVerse
import QtQuick.Layouts

Item {
    id: root

    RowLayout {
        anchors.centerIn: parent
        width: Math.min(1100, parent.width)
        height: Math.min(560, parent.height)
        spacing: 40

        // Ring with the overall percentage.
        Item {
            Layout.preferredWidth: 320
            Layout.fillHeight: true
            ProgressRing {
                id: ring
                anchors.centerIn: parent
                width: 300; height: 300
                value: app.overallFraction
                busy: app.phase === "running"
            }
            Column {
                anchors.centerIn: parent
                spacing: 2
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: Math.round(ring.shown * 100) + "%"
                    color: Theme.text; font.pixelSize: 54; font.weight: Font.Bold; font.features: { "tnum": 1 }
                }
                Text {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: app.stage; color: Theme.muted; font.pixelSize: 14
                }
            }
        }

        // Stages and the universe grid.
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 22
            Text { text: qsTr("Running every defensible analysis"); color: Theme.text; font.pixelSize: 26; font.weight: Font.DemiBold }
            Text { text: app.experimentName; color: Theme.muted; font.pixelSize: 14 }
            StageList { Layout.fillWidth: true; current: app.stageIndex; fraction: app.stageFraction }
            GlassCard {
                Layout.fillWidth: true
                Layout.fillHeight: true
                ColumnLayout {
                    anchors { fill: parent; margins: 18 }
                    spacing: 8
                    Text {
                        text: app.stageIndex >= 3 ? qsTr("%1 universes · permuting group labels within blots").arg(app.universeTotal)
                                                  : qsTr("%1 universes").arg(app.universeTotal)
                        color: Theme.muted; font.pixelSize: 12
                    }
                    UniverseGrid {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        count: Math.max(1, app.universeTotal)
                        litFraction: app.stageIndex > 2 ? 1 : (app.stageIndex === 2 ? app.stageFraction : 0)
                        waving: app.stageIndex === 3
                    }
                }
            }
        }
    }
}
