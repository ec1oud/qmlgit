import QtQuick
import QtQuick.Controls

Rectangle {
    id: root
    width: parent.width
    height: messageLabel.height * 2 + 4
    color: "#DCDCDC"
    border.color: "#303030"
    border.width: focus ? 2 : 1
    radius: 4

    property string oid
    property alias message: messageLabel.text
    property alias author: authorLabel.text

    signal dragCommit(int row, string oid)
    signal dropTarget(int row, string oid)
    signal dragAndDropFinished()

    Label {
        y: 2
        id: messageLabel
        width: parent.width - 4
        elide: Text.ElideRight
        x: 3
    }
    Label {
        y: messageLabel.height + 2
        id: authorLabel
        width: parent.width - 4
        elide: Text.ElideRight
        x: 3
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        drag.target: parent
        onPressed: parent.focus = true
        drag.onActiveChanged: {
            if (drag.active)
                root.dragCommit(index, root.oid)
            else
                root.dragAndDropFinished()
        }
    }

    Drag.active: mouseArea.drag.active
    z: Drag.active ? 1 : 0
    Drag.hotSpot.x: 10
    Drag.hotSpot.y: 10

    DropArea {
        enabled: !parent.focus
        anchors.fill: parent
        onContainsDragChanged: {
            parent.opacity = containsDrag ? 0.7 : 1.0
            root.dropTarget(index, root.oid)
        }
    }
}
