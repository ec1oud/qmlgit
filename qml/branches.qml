import QtQuick 2.0
import Git 1.0
import CustomGeometry 1.0

Item {
    anchors.fill: parent

    Git {
        id: git
        repoUrl: root.repoUrl
    }

    Repeater {
        model: git.branches
        delegate: Branch {
            name: modelData
            x: 10 + index * 150
            y: 10
        }
    }

}
