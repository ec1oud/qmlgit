import QtQuick 2.0

import QtDesktop 1.0

ApplicationWindow {
    width: 600
    height: 400

    id: root
    property alias repoUrl: urlField.text

    Action {
        shortcut: "1"
        onTriggered: contents.source = "qrc:/qml/diff.qml"
    }
    Action {
        shortcut: "2"
        onTriggered: contents.source = "qrc:/qml/log.qml"
    }

    toolBar: ToolBar {
        width: parent.width
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            Label { text: "Repo:" }
            TextField {
                id: urlField
                text: "/home/frederik/dev/qt-src-dev/qtbase"
                Layout.horizontalSizePolicy: Layout.Expanding
            }
            Button {
                text: "Diff"
                onClicked: {
                    contents.source = "qrc:/qml/diff.qml"
                }
            }
            Button {
                text: "Log"
                onClicked: {
                    contents.source = "qrc:/qml/log.qml"
                }
            }
        }
    }

    Loader {
        id: contents
        anchors.fill: parent
        anchors.margins: 4
        source: "qrc:/qml/log.qml"
    }
}
