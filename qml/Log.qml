import QtQuick 2.0
import QtDesktop 1.0
import Git 1.0

Item {
    anchors.fill: parent

    Git {
        id: git
        repoUrl: root.repoUrl
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        RowLayout {
            width: parent.width
            height: 40
            spacing: 4

            Label {
                text: "Branch:"
            }
            ComboBox {
                id: comboBox
                model: git.branches
                width: 150
            }
            Item {
                Layout.horizontalSizePolicy: Layout.Expanding
            }
        }

        Row {
            width: parent.width
            Layout.verticalSizePolicy: Layout.Expanding
            LogView {
                id: logView
                width: details ? parent.width/3 : parent.width
                height: parent.height

                repoUrl: git.repoUrl
                branch: comboBox.selectedText
                Behavior on width { NumberAnimation { duration: 100} }
            }
            DiffView {
                visible: logView.details
                width: parent.width/3*2
                height: parent.height
                commit: logView.currentCommit
            }
        }
    }
}

