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

import Git 1.0

Rectangle {
    SystemPalette {
        id: pal
    }
//    color: pal.light

    GitCommitList {
        id: gitLog
        repoUrl: root.repoUrl
    }

    TableView {
        id: table
        model: gitLog
        height: parent.height
        width: parent.width/4*3

        TableColumn {
            role: "shortMessage"
            title: "Commit"
            width: 300
        }
        TableColumn {
            role: "author"
            title: "Author"
            width: 100
        }
    }

//    ListView {
//        id: list
//        height: parent.height
//        width: parent.width/4*3
//        clip: true
//        focus: true

//        model: gitLog
//        delegate: commitDelegate
//        highlight: Rectangle { color: "lightsteelblue"; radius: 5 }
////        highlightFollowsCurrentItem: true


//    }

    Text {
        anchors.right: parent.right
        text: "Total: " + table.count
    }

//    Component {
//        id: commitDelegate

//        Item {
//            x: 4
//            width: parent.width - 8
//            height: messageText.height + 16

//            Behavior on height { NumberAnimation {} }

//            property bool details: false

//            Rectangle {
//                anchors.fill: parent
//                anchors.margins: 4
//                radius: 3
//                color: pal.light
//                Text {
//                    id: messageText
//                    x: 4
//                    width: parent.width - 8
//                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
//                    y: 4
//                    text: details ? message.concat("\n ", author, " <", authorEmail, ">", time)
//                                  : shortMessage + "\n  " + author
//                }
//            }

//            MouseArea {
//                anchors.fill: parent
//                onClicked: {
//                    details = !details
//                    list.currentIndex = index
//                }
//            }
//        }
//    }

}
