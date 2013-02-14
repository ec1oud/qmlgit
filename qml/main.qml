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
                text: "/home/frederik/dev/projects/qmlgit"
                onTextChanged: {
                    Git.repo = repo.text
                }
                Layout.horizontalSizePolicy: Layout.Expanding
            }

        }
    }

    TextArea {
        anchors.fill: parent
        text: Git.diff
    }
}
