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
                text: "Branches:"
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
        Log {
            width: parent.width

            repoUrl: git.repoUrl
            branch: comboBox.selectedText
            Layout.verticalSizePolicy: Layout.Expanding
        }
    }
}

