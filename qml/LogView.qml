import QtQuick 2.0

import Git 1.0

Item {
    id: logView
    property alias repoUrl: gitLog.repoUrl
    property alias branch: gitLog.currentBranch
    property alias currentItem: list.currentItem
    property bool details: false
    property string currentCommit

    property string author
    property string authorEmail
    property string date
    property string committer
    property string committerEmail
    property string message

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

    Component {
        id: commitDelegate

        Item {
            width: parent.width - 8
            height: messageText.height + 8

            property bool current: ListView.isCurrentItem
            onCurrentChanged: {
                if (current) {
                    logView.author = author
                    logView.message = message
                    logView.authorEmail = authorEmail
                    logView.date = date
                    logView.committer = committer
                    logView.committerEmail = committerEmail
                    logView.currentCommit = oid
                }
            }

            Rectangle {
                anchors.fill: parent
                color: current ? pal.highlight : (index % 2 ? pal.base : pal.alternateBase)
                Text {
                    id: messageText
                    x: 4
                    width: parent.width - 8
//                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    elide: Text.ElideRight
                    y: 4
                    text: shortMessage
                    color: current ? pal.highlightedText : pal.text
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (list.currentIndex == index) {
                        logView.details = !logView.details
                    } else {
                        list.currentIndex = index
                        logView.details = true
                    }
                }
            }
        }
    }

}
