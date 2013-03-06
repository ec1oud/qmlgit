import QtQuick 2.0
import CustomGeometry 1.0

Rectangle {
    id: rect
    width: 120
    height: 62

    color: "lightgreen"
    border.color: "darkgreen"
    border.width: 2
    radius: 10

    property alias text: t.text

    Text {
        id: t
        anchors.centerIn: parent
    }

    MouseArea {
        anchors.fill: parent
        drag.target: parent
    }
}
