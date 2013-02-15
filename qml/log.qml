import QtQuick 2.0

Rectangle {
    color: "#005500aa"

    ListView {
        model: Git.log
        delegate: commitDelegate
        height: parent.height
        width: parent.width/2
    }

    Component {
        id: commitDelegate

        Rectangle {
            width: parent.width
            height: 40
            radius: 3
            color: "lightsteelblue"
            Text {
                anchors.centerIn: parent
                text: "Commit"
            }
        }
    }

}
