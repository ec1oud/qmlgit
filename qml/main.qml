import QtQuick 2.0

import QtDesktop 1.0

ApplicationWindow {
    width: 600
    height: 400

    id: root
    property alias repoUrl: urlField.text

    Action {
        shortcut: "ctrl+1"
        onTriggered: tabs.current = 0
    }
    Action {
        shortcut: "ctrl+2"
        onTriggered: tabs.current = 1
    }
    Action {
        shortcut: "ctrl+3"
        onTriggered: tabs.current = 2
    }

    toolBar: ToolBar {
        width: parent.width
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            Label { text: "Repo:" }
            TextField {
                id: urlField
//                text: "/home/frederik/dev/tools/git/qmlgit"
//                text: "/home/frederik/dev/qt-src-dev/qtbase"
                text: workingDir
                Layout.horizontalSizePolicy: Layout.Expanding
            }
        }
    }

    TabFrame {
        id: tabs
        anchors.fill: parent
        width: parent.width

        current: 1
        Tab {
            title: "Status"
            Loader {
                anchors.fill: parent
                source: visible ? "qrc:/qml/status.qml" : ""
            }
        }
        Tab {
            title: "Log"
            Loader {
                anchors.fill: parent
                source: visible ? "qrc:/qml/Log.qml" : ""
            }
        }
        Tab {
            title: "Diff"
            Loader {
                anchors.fill: parent
                source: visible ? "qrc:/qml/diff.qml" : ""
            }
        }
    }

    statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            Label {
                text: "Loading..."
            }
            Item {
                Layout.horizontalSizePolicy: Layout.Expanding
            }
        }
    }
}
