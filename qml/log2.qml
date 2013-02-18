import QtQuick 2.0
import QtDesktop 1.0

import Git 1.0

Rectangle {
    SystemPalette {
        id: pal
    }
//    color: pal.light

    GitCommitList {
        id: gitLog
        repoUrl: root.repoUrl
    }

    TableView {
        id: table
        model: gitLog
        height: parent.height
        width: parent.width/4*3

        TableColumn {
            role: "shortMessage"
            title: "Commit"
            width: 300
        }
        TableColumn {
            role: "author"
            title: "Author"
            width: 100
        }
    }

//    ListView {
//        id: list
//        height: parent.height
//        width: parent.width/4*3
//        clip: true
//        focus: true

//        model: gitLog
//        delegate: commitDelegate
//        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
////        highlightFollowsCurrentItem: true


//    }

    Text {
        anchors.right: parent.right
        text: "Total: " + table.count
    }

//    Component {
//        id: commitDelegate

//        Item {
//            x: 4
//            width: parent.width - 8
//            height: messageText.height + 16

//            Behavior on height { NumberAnimation {} }

//            property bool details: false

//            Rectangle {
//                anchors.fill: parent
//                anchors.margins: 4
//                radius: 3
//                color: pal.light
//                Text {
//                    id: messageText
//                    x: 4
//                    width: parent.width - 8
//                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
//                    y: 4
//                    text: details ? message.concat("\n ", author, " <", authorEmail, ">", time)
//                                  : shortMessage + "\n  " + author
//                }
//            }

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    details = !details
//                    list.currentIndex = index
//                }
//            }
//        }
//    }

}
