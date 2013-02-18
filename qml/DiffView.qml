import QtQuick 2.0
import QtDesktop 1.0

import DiffHighlighter 1.0
import Git 1.0

Rectangle {

    property alias commit: git.currentCommit

    Git {
        id: git
        repoUrl: root.repoUrl
        onRepoUrlChanged: console.log(repoUrl)
    }

    TextArea {
        id: textArea
        font.family: "mono"
        anchors.fill: parent
        text: git.diff
        readOnly: true
    }

    DiffHighlighter {
        target: textArea
    }
}
