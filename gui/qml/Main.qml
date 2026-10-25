import QtQuick
import BlotVerse
import QtQuick.Controls.Basic
import QtQuick.Effects
import QtQuick.Layouts

ApplicationWindow {
    id: window
    width: 1320
    height: 860
    minimumWidth: 1080
    minimumHeight: 720
    visible: true
    title: qsTr("BlotVerse")
    color: Theme.bg0
    font.family: Theme.font

    // --- Living background: two slow, blurred colour fields -----------------
    Item {
        id: aurora
        anchors.fill: parent
        layer.enabled: true
        layer.effect: MultiEffect { blurEnabled: true; blur: 1.0; blurMax: 64 }

        Rectangle {
            width: parent.width * 0.7; height: width; radius: width / 2
            color: Theme.accent; opacity: 0.12
            x: -width * 0.25 + Math.sin(drift.t) * 60
            y: -height * 0.45 + Math.cos(drift.t * 0.7) * 40
        }
        Rectangle {
            width: parent.width * 0.6; height: width; radius: width / 2
            color: Theme.accent2; opacity: 0.10
            x: parent.width - width * 0.6 + Math.cos(drift.t * 0.8) * 70
            y: parent.height - height * 0.5 + Math.sin(drift.t * 0.6) * 50
        }
        QtObject {
            id: drift
            property real t: 0
            NumberAnimation on t { from: 0; to: Math.PI * 2; duration: 60000; loops: Animation.Infinite }
        }
    }

    // --- Header ---------------------------------------------------------------
    RowLayout {
        id: header
        anchors { left: parent.left; right: parent.right; top: parent.top; margins: 24 }
        height: 44
        spacing: 14

        Text {
            text: "BlotVerse"
            font.pixelSize: 26; font.weight: Font.DemiBold; font.family: Theme.font
            color: Theme.text
            layer.enabled: true
            layer.effect: MultiEffect {
                colorization: 0.0
                shadowEnabled: true; shadowColor: Theme.accent; shadowBlur: 0.6; shadowOpacity: 0.35
            }
        }
        Text { text: "v" + appVersion; color: Theme.faint; font.pixelSize: 13; Layout.alignment: Qt.AlignBottom; bottomPadding: 4 }

        Item { Layout.fillWidth: true }

        Repeater {
            model: [qsTr("Start"), qsTr("Setup"), qsTr("Analysis"), qsTr("Results")]
            delegate: Row {
                spacing: 8
                required property int index
                required property string modelData
                readonly property int current: ["start", "setup", "running", "results"].indexOf(app.phase)
                Rectangle {
                    width: 8; height: 8; radius: 4; anchors.verticalCenter: parent.verticalCenter
                    color: index <= current ? Theme.accent : Theme.faint
                    Behavior on color { ColorAnimation { duration: Theme.normal } }
                }
                Text {
                    text: modelData; font.pixelSize: 14
                    color: index === current ? Theme.text : Theme.muted
                    Behavior on color { ColorAnimation { duration: Theme.normal } }
                }
                Text { visible: index < 3; text: "—"; color: Theme.faint; font.pixelSize: 14 }
            }
        }
    }

    // --- Pages: cross-fade and lift between phases ------------------------------
    Item {
        id: pages
        anchors { left: parent.left; right: parent.right; top: header.bottom; bottom: parent.bottom; margins: 24 }

        component Page: Item {
            required property string phaseName
            anchors.fill: parent
            readonly property bool active: app.phase === phaseName
            opacity: active ? 1 : 0
            visible: opacity > 0.01
            enabled: active
            transform: Translate { y: active ? 0 : 24; Behavior on y { NumberAnimation { duration: Theme.normal; easing.type: Easing.OutCubic } } }
            // The outgoing page leaves quickly; the incoming one waits for it.
            Behavior on opacity {
                SequentialAnimation {
                    PauseAnimation { duration: active ? 160 : 0 }
                    NumberAnimation { duration: active ? Theme.normal : 160; easing.type: Easing.OutCubic }
                }
            }
        }

        Page { phaseName: "start"; StartPage { anchors.fill: parent } }
        Page { phaseName: "setup"; SetupPage { anchors.fill: parent } }
        Page { phaseName: "running"; RunningPage { anchors.fill: parent } }
        Page { phaseName: "results"; ResultsPage { anchors.fill: parent } }
    }

    // --- Errors -----------------------------------------------------------------
    Rectangle {
        anchors.fill: parent
        color: "#aa000000"
        visible: app.phase === "error"
        opacity: visible ? 1 : 0
        Behavior on opacity { NumberAnimation { duration: Theme.fast } }
        MouseArea { anchors.fill: parent }

        GlassCard {
            anchors.centerIn: parent
            width: Math.min(560, parent.width - 80)
            height: errorColumn.implicitHeight + 56
            ColumnLayout {
                id: errorColumn
                anchors { fill: parent; margins: 28 }
                spacing: 14
                Text { text: qsTr("Something needs attention"); color: Theme.unsupported; font.pixelSize: 20; font.weight: Font.DemiBold }
                Text {
                    text: app.errorMessage
                    color: Theme.text; wrapMode: Text.WordWrap; font.pixelSize: 14
                    Layout.fillWidth: true
                }
                RowLayout {
                    Layout.alignment: Qt.AlignRight
                    AccentButton { text: qsTr("Back"); subtle: true; onClicked: app.experimentName ? app.backToSetup() : app.reset() }
                }
            }
        }
    }
}
