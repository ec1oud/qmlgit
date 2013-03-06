import QtQuick 2.0
import CustomGeometry 1.0

Item {
    id: r
    property string name

    property bool moving: Math.abs(box1.x - box2.x) > 50

    Box {
        id: box1
        text: r.name
    }
    Box {
        id: box2
        y: 100
       // opacity: moving ? 0.5 : 1
    }
    BezierCurve {
        item1: box1
        item2: box2
       // visible: !moving
    }
}
