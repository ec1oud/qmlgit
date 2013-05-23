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

import QtQuick 2.0
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0
import Git 1.0

ApplicationWindow {
    width: 600
    height: 400

    id: root
    Git {
        id: git
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

    toolBar: ToolBar {
        width: parent.width
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            Label { text: "Repo:" }
            TextField {
                id: urlField
//                text: "/home/frederik/dev/tools/git/qmlgit"
//                text: "/home/frederik/dev/qt-src-dev/qtbase"
                text: workingDir
                Layout.horizontalSizePolicy: Layout.Expanding
            }
        }
    }

    TabView {
        id: tabs
        anchors.fill: parent
        width: parent.width

        currentIndex: 1
        Tab {
            title: "Status"
            Loader {
                anchors.fill: parent
                source: visible ? "qrc:/qml/status.qml" : ""
            }
        }
        Tab {
            title: "Log"
            Loader {
                anchors.fill: parent
                source: visible ? "qrc:/qml/Log.qml" : ""
            }
        }
        Tab {
            title: "Diff"
            Loader {
                anchors.fill: parent
                source: visible ? "qrc:/qml/diff.qml" : ""
            }
        }

        Tab {
            title: "Branches"
            Loader {
                anchors.fill: parent
                source: visible ? "qrc:/qml/branches.qml" : ""
            }
        }
    }

    statusBar: StatusBar {
        RowLayout {
            anchors.fill: parent
            anchors.margins: 4
            Label {
                text: git.statusMessage
            }
            Item {
                Layout.horizontalSizePolicy: Layout.Expanding
            }
        }
    }
}
