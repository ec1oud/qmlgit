import QtQuick 2.0

import Git 1.0

Rectangle {
    SystemPalette {
        id: pal
    }
    color: pal.light

    GitCommitList {
        id: gitLog
        repoUrl: root.repoUrl
    }

    ListView {
        id: list
        height: parent.height
        width: parent.width/4*3
        clip: true
        focus: true

        model: gitLog
        delegate: commitDelegate
        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
    }

    Text {
        anchors.right: parent.right
        text: "Total: " + list.count
    }

    Component {
        id: commitDelegate

        Item {
            height: 50
            x: 4
            width: parent.width - 8

            Rectangle {
                width: parent.width
                height: 40
                radius: 3
                color: pal.midlight
                Text {
                    x: 4
                    width: parent.width - 8
                    y: 4
                    text: shortMessage + "\n  " + author
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {}
            }
        }
    }

}
