import QtQuick 2.0

import QtDesktop 1.0

ApplicationWindow {
    width: 600
    height: 400

    toolBar: ToolBar {
        width: parent.width
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            Label { text: "Repo:" }
            TextField {
                id: repo
                text: "/home/frederik/dev/tools/git/qmlgit"
                onTextChanged: {
                    Git.repo = repo.text
                }
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
        anchors.fill: parent
        anchors.margins: 4
        id: contents
    }
}
