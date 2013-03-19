import QtQuick 2.0
import QtQuick.Controls 1.0
import Git 1.0

Item {
    anchors.fill: parent

    ListView {
        anchors.fill: parent
//        model: git.branches
        delegate: Item {
            height: 30
            width: 100
            CheckBox {
                anchors.fill: parent
                text: modelData
            }
        }
    }
}

