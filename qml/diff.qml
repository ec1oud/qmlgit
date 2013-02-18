import QtQuick 2.0
import QtDesktop 1.0

import DiffHighlighter 1.0
import Git 1.0

Rectangle {

    GitDiff {
        id: diff
        repoUrl: root.repoUrl
        onRepoUrlChanged: console.log(repoUrl)
    }

    TextArea {
        id: textArea
        anchors.fill: parent
        text: diff.diff
    }

    DiffHighlighter {
        target: textArea
    }

}
