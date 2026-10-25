import QtQuick
import BlotVerse
import QtQuick.Controls.Basic
import QtQuick.Dialogs
import QtQuick.Layouts

Item {
    id: root
    readonly property var r: app.result

    FolderDialog {
        id: exportDialog
        title: qsTr("Choose a folder for the report")
        onAccepted: app.exportResults(selectedFolder)
    }

    Flickable {
        anchors.fill: parent
        contentHeight: column.implicitHeight
        clip: true
        boundsBehavior: Flickable.StopAtBounds

        ColumnLayout {
            id: column
            width: parent.width
            spacing: 18

            // Verdict.
            RowLayout {
                Layout.fillWidth: true
                spacing: 18
                Rectangle {
                    id: badge
                    Layout.preferredHeight: 64
                    Layout.preferredWidth: verdictText.implicitWidth + 56
                    radius: 32
                    color: Theme.verdictColor(root.r.verdictKey ?? 3)
                    scale: 0
                    Text { id: verdictText; anchors.centerIn: parent; text: root.r.verdict ?? ""; color: "#0b1220"; font.pixelSize: 28; font.weight: Font.Bold }
                    Connections { target: app; function onResultChanged() { pop.restart() } }
                    SequentialAnimation {
                        id: pop
                        running: true
                        PauseAnimation { duration: 150 }
                        NumberAnimation { target: badge; property: "scale"; from: 0.4; to: 1; duration: 650; easing.type: Easing.OutBack; easing.overshoot: 2.2 }
                    }
                }
                ColumnLayout {
                    spacing: 2
                    Text { text: root.r.contrast ?? ""; color: Theme.text; font.pixelSize: 22; font.weight: Font.DemiBold }
                    Text { text: root.r.verdictReason ?? ""; color: Theme.text; font.pixelSize: 14; wrapMode: Text.WordWrap; Layout.maximumWidth: 900 }
                    Text {
                        text: qsTr("%1 valid universes of %2 · %3 excluded by the spec · %4 joint permutations · %5 s")
                              .arg(root.r.nValid ?? 0).arg(root.r.nUniverses ?? 0).arg(root.r.nExcluded ?? 0)
                              .arg(root.r.permutations ?? 0).arg(Theme.fmt(root.r.seconds, 1))
                        color: Theme.muted; font.pixelSize: 13
                    }
                }
                Item { Layout.fillWidth: true }
                ComboBox {
                    visible: app.contrasts.length > 1
                    model: app.contrasts
                    currentIndex: app.contrastIndex
                    onActivated: i => app.contrastIndex = i
                }
            }

            // Notes from the analysis (e.g. design too small for the joint test).
            Repeater {
                model: root.r.notes ?? []
                delegate: Text { required property string modelData; text: "ⓘ " + modelData; color: Theme.fragile; font.pixelSize: 13; wrapMode: Text.WordWrap; Layout.fillWidth: true }
            }

            // Headline numbers.
            GridLayout {
                Layout.fillWidth: true
                columns: 3
                rowSpacing: 14; columnSpacing: 14
                StatTile { Layout.fillWidth: true; title: qsTr("Robustness R"); value: root.r.robustness ?? NaN; digits: 3
                           caption: qsTr("same direction and p < 0.05 as the default") }
                StatTile { Layout.fillWidth: true; title: qsTr("Joint permutation test"); text: Theme.fmtP(root.r.jointP)
                           caption: root.r.jointExact ? qsTr("exact (all relabellings)") : qsTr("Monte Carlo") }
                StatTile { Layout.fillWidth: true; title: qsTr("Default analysis"); value: root.r.defaultLfc ?? NaN; digits: 3
                           caption: qsTr("log2 FC · p %1").arg(Theme.fmtP(root.r.defaultP)) }
                StatTile { Layout.fillWidth: true; title: qsTr("Median effect"); value: root.r.medianLfc ?? NaN; digits: 3
                           caption: qsTr("5–95 %: %1 to %2").arg(Theme.fmt(root.r.lfcLow, 2)).arg(Theme.fmt(root.r.lfcHigh, 2)) }
                StatTile { Layout.fillWidth: true; title: qsTr("Opposite sign"); value: root.r.opposite ?? NaN; digits: 3
                           caption: qsTr("fraction of universes") }
                StatTile { Layout.fillWidth: true; title: qsTr("Moves the result most"); text: root.r.mostInfluential || "–"
                           caption: qsTr("largest share of effect variance") }
            }

            // Specification curve and influence.
            RowLayout {
                Layout.fillWidth: true
                spacing: 18
                GlassCard {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 520
                    ColumnLayout {
                        anchors { fill: parent; margins: 18 }
                        spacing: 8
                        Text { text: qsTr("Specification curve"); color: Theme.text; font.pixelSize: 17; font.weight: Font.DemiBold }
                        Text {
                            text: qsTr("Every universe ordered by effect. Blue: significant in the default's direction · red: opposite · grey: not significant · ringed: pre-registered default.")
                            color: Theme.faint; font.pixelSize: 12; wrapMode: Text.WordWrap; Layout.fillWidth: true
                        }
                        SpecCurve { Layout.fillWidth: true; Layout.fillHeight: true; curve: root.r.curve ?? []; panel: root.r.panel ?? [] }
                    }
                }
                GlassCard {
                    Layout.preferredWidth: 340
                    Layout.preferredHeight: 520
                    ColumnLayout {
                        anchors { fill: parent; margins: 18 }
                        spacing: 12
                        Text { text: qsTr("What moves the result"); color: Theme.text; font.pixelSize: 17; font.weight: Font.DemiBold }
                        Text { text: qsTr("Share of the effect's variance explained by each decision (η²)."); color: Theme.faint; font.pixelSize: 12; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                        InfluenceBars { Layout.fillWidth: true; bars: root.r.influence ?? [] }
                        Item { Layout.fillHeight: true }
                    }
                }
            }

            // Quality control.
            GlassCard {
                Layout.fillWidth: true
                Layout.preferredHeight: qcColumn.implicitHeight + 36
                ColumnLayout {
                    id: qcColumn
                    anchors { left: parent.left; right: parent.right; top: parent.top; margins: 18 }
                    spacing: 6
                    Text { text: qsTr("Quality control"); color: Theme.text; font.pixelSize: 17; font.weight: Font.DemiBold }
                    Text { visible: (root.r.warnings ?? []).length === 0; text: qsTr("No warnings."); color: Theme.muted; font.pixelSize: 13 }
                    Repeater {
                        model: root.r.warnings ?? []
                        delegate: Text { required property string modelData; text: "• " + modelData; color: Theme.muted; font.pixelSize: 13; wrapMode: Text.WordWrap; Layout.fillWidth: true }
                    }
                }
            }

            // Actions.
            RowLayout {
                Layout.fillWidth: true
                Layout.bottomMargin: 8
                spacing: 10
                AccentButton { text: qsTr("Export report…"); large: true; onClicked: exportDialog.open() }
                AccentButton { text: qsTr("Open report"); subtle: true; visible: app.lastExport !== ""; onClicked: app.openLastReport() }
                Text { text: app.lastExport; color: Theme.faint; font.pixelSize: 12; elide: Text.ElideMiddle; Layout.fillWidth: true }
                AccentButton { text: qsTr("Back to setup"); subtle: true; onClicked: app.backToSetup() }
                AccentButton { text: qsTr("New analysis"); subtle: true; onClicked: app.reset() }
            }
        }
    }
}
