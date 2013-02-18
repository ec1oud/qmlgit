import QtQuick 2.0

import Git 1.0

Rectangle {
    SystemPalette {
        id: pal
    }

    Git {
        id: gitLog
        repoUrl: root.repoUrl
    }

    ListView {
        id: list
        height: parent.height
        width: parent.width/4*3
        clip: true
        focus: true

        model: gitLog.logModel
        delegate: commitDelegate

        Keys.onDownPressed: incrementCurrentIndex()
        Keys.onUpPressed: decrementCurrentIndex()

        Component.onCompleted: forceActiveFocus()
    }

    Text {
        anchors.right: parent.right
        text: "Total: " + list.count
    }

    Component {
        id: commitDelegate

        Item {
            x: 4
            width: parent.width - 8
            height: messageText.height + 16

            Behavior on height { NumberAnimation { duration: 200 } }

            property bool details: ListView.isCurrentItem

            Rectangle {
                anchors.fill: parent
                anchors.margins: 4
                radius: 3
                color: details ? pal.highlight : pal.base
                Text {
                    id: messageText
                    x: 4
                    width: parent.width - 8
                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    y: 4
                    text: details ? message.concat("\n ", author, " <", authorEmail, ">\n", time)
                                  : shortMessage + "\n  " + author
                    color: details ? pal.highlightedText : pal.text
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    list.currentIndex = index
                    forceActiveFocus()
                }
            }
        }
    }

}
