import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
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
                implicitWidth: 200
            }
            Label {
                id: authorLabel
                text: (logView.author.length > 0) ? logView.author.concat(" <", logView.authorEmail, ">") : ""
                horizontalAlignment: Text.AlignRight
                Layout.horizontalSizePolicy: Layout.Expanding
            }
        }

        Row {
            height: parent.height - 40
            Layout.verticalSizePolicy: Layout.Expanding
            Layout.horizontalSizePolicy: Layout.Expanding

            LogView {
                id: logView
                width: details ? parent.width/3 : parent.width
                height: parent.height

                repoUrl: git.repoUrl
                branch: comboBox.selectedText
                Behavior on width { NumberAnimation { duration: 100} }
            }
            SplitView {
                id: splitter
                orientation: Qt.Vertical
                visible: logView.details
                width: parent.width/3*2
                height: parent.height
                ScrollView {
                    frameVisible: true
                    id: commitView
                    width: parent.width
                    height: 120
                    Text {
                        x: 4
                        y: 4
                        width: splitter.width - 40
                        text: logView.message.concat("\n", logView.date)
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                }
                DiffView {
                    width: parent.width
                    height: 120
                    commit: logView.currentCommit
                }
            }
        }
    }
}

