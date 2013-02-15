import QtQuick 2.0
import QtDesktop 1.0

import DiffHighlighter 1.0

Rectangle {
    TextArea {
        id: textArea
        anchors.fill: parent
        text: Git.diff
    }

    DiffHighlighter {
        target: textArea
    }

}
