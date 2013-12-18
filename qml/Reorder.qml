import QtQuick 2.1
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1

import Git 1.0

RowLayout {
    anchors.fill: parent


    ColumnLayout {
        height: parent.height
        width: parent.width / 3
        ComboBox {
            id: branchCombo
            model: gitRepo.branches
        }
        GitModel {
            id: gitModel
            git: gitRepo
            currentBranch: branchCombo.currentText
        }
        ScrollView {
            frameVisible: true
            Layout.fillHeight: true

            ListView {
                anchors.fill: parent
                model: gitModel
                delegate: CommitRect {
                    message: shortMessage
                    author: model.author
                    oid: model.oid
                    onDragCommit: {
                        console.log("Drag " + oid)
                        git.startDrag(oid)
                    }
                }
            }
        }
    }

    ColumnLayout {
        height: parent.height
        width: parent.width / 3
        ComboBox {
            id: branchCombo2
            model: gitRepo.branches
        }
        GitModel {
            id: gitModel2
            git: gitRepo
            currentBranch: branchCombo2.currentText
        }
        ScrollView {
            frameVisible: true
            Layout.fillHeight: true

            ListView {
                anchors.fill: parent
                model: gitModel2
                delegate: CommitRect {
                    message: shortMessage
                    author: model.author
                    oid: model.oid
                    onDragCommit: {
                        console.log("Drag " + oid)
                        git.startDrag(oid)
                    }
                }
            }
        }
    }

}
