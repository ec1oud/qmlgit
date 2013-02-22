import QtQuick 2.0
import QtQuick.Controls 1.0

import DiffHighlighter 1.0
import Git 1.0

Rectangle {

    Git {
        id: git
        repoUrl: root.repoUrl
        onRepoUrlChanged: console.log(repoUrl)
    }

    TextArea {
        id: textArea
        anchors.fill: parent
        text: git.diff
        readOnly: true
    }

    DiffHighlighter {
        target: textArea
    }
}
