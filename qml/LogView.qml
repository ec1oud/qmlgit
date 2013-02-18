import QtQuick 2.0

import Git 1.0

Item {
    id: logView
    property alias repoUrl: gitLog.repoUrl
    property alias branch: gitLog.currentBranch
    property alias currentItem: list.currentItem
    property bool details: false
    property string currentCommit: ""

    SystemPalette {
        id: pal
    }

    Git {
        id: gitLog
        repoUrl: log.repoUrl
    }

    ListView {
        id: list
        anchors.fill: parent
        clip: true
        focus: true

        model: gitLog.logModel
        delegate: commitDelegate

        Keys.onDownPressed: incrementCurrentIndex()
        Keys.onUpPressed: decrementCurrentIndex()

        Component.onCompleted: forceActiveFocus()
    }

//    Text {
//        anchors.right: parent.right
//        text: "Total: " + list.count
//    }

    Component {
        id: commitDelegate

        Item {
            x: 4
            width: parent.width - 8
            height: messageText.height + 16

            Behavior on height { NumberAnimation { duration: 200 } }

            property bool current: ListView.isCurrentItem

            Rectangle {
                anchors.fill: parent
                anchors.margins: 4
                radius: 3
                color: current ? pal.highlight : pal.base
                Text {
                    id: messageText
                    x: 4
                    width: parent.width - 8
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    y: 4
                    text: current & logView.details ? message.concat("\n ", author, " <", authorEmail, ">\n", time, "\n", oid)
                                  : shortMessage + "\n  " + author
                    color: current & logView.details ? pal.highlightedText : pal.text
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (list.currentIndex == index) {
                        logView.details = !logView.details
                    } else {
                        list.currentIndex = index
                        logView.currentCommit = oid
                    }
                }
            }
        }
    }

}
