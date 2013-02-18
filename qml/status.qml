import QtQuick 2.0
import QtDesktop 1.0
import Git 1.0

Rectangle {
    anchors.fill: parent

    Git {
        id: git
        repoUrl: root.repoUrl
    }

    Label {
        text: "Branches: " + git.branches
        Component.onCompleted: console.log(git.branches.toString())
    }
    ComboBox {
        model: git.branches
    }
}

