import QtQuick 2.0

import QtQuick.Controls 1.0

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

    TabView {
        id: tabs
        anchors.fill: parent
        width: parent.width

        currentIndex: 1
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

        Tab {
            title: "Branches"
            Loader {
                anchors.fill: parent
                source: visible ? "qrc:/qml/branches.qml" : ""
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
