import QtQuick 2.0
import QtQuick.Controls 1.0

import DiffHighlighter 1.0
import Git 1.0

Rectangle {

    TextArea {
        id: textArea
        font.family: "mono"
        anchors.fill: parent
        text: git.diff
        readOnly: true
        selectByKeyboard: true
    }

    DiffHighlighter {
        target: textArea
    }
}
