/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of qmlgit.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Git

ApplicationWindow {
    width: 600
    height: 400

    id: root
    Git {
        id: gitRepo
        repoUrl: urlField.text
    }

    Action {
        shortcut: "ctrl+1"
        onTriggered: tabs.current = 0
    }
    Action {
        shortcut: "ctrl+2"
        onTriggered: tabs.current = 1
    }
    Action {
        shortcut: "ctrl+3"
        onTriggered: tabs.current = 2
    }

    header: ToolBar {
        height: tbLayout.implicitHeight + 8
        RowLayout {
            id: tbLayout
            anchors.fill: parent
            anchors.margins: 4
            Label { text: "Repo:" }
            TextField {
                id: urlField
//                text: "/home/frederik/dev/tools/git/qmlgit"
//                text: "/home/frederik/dev/qt-src-dev/qtbase"
                text: workingDir
                Layout.fillWidth: true
            }
        }
    }

    TabBar {
        id: bar
        width: parent.width
        TabButton {
            text: qsTr("Branches")
        }
        TabButton {
            text: qsTr("Reorder")
        }
        TabButton {
            text: qsTr("Log")
        }
    }

    Loader {
        anchors.fill: parent
        anchors.topMargin: bar.height
        source:
            switch (bar.currentIndex) {
            case 0:
                return "qrc:/qml/Branch.qml"
            case 1:
                return "qrc:/qml/Reorder.qml"
            case 2:
                return "qrc:/qml/Log.qml"
            }
    }

    footer: ToolBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            Label {
                text: gitRepo.statusMessage
            }
            Item {
                Layout.fillWidth: true
            }
        }
    }
}
