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

import QtQuick 2.1

import Git 1.0

Item {
    id: logView
    property alias currentItem: list.currentItem
    property bool details: false
    property string currentCommit

    property string author
    property string authorEmail
    property string date
    property string committer
    property string committerEmail
    property string message

    SystemPalette {
        id: pal
    }

    ListView {
        id: list
        anchors.fill: parent
        clip: true
        focus: true
        activeFocusOnTab: true

        model: git.logModel
        delegate: commitDelegate

        Keys.onDownPressed: incrementCurrentIndex()
        Keys.onUpPressed: decrementCurrentIndex()

        Component.onCompleted: forceActiveFocus()
    }

    Component {
        id: commitDelegate

        Item {
            width: parent.width - 8
            height: messageText.height + 8

            property bool current: ListView.isCurrentItem
            onCurrentChanged: {
                if (current) {
                    logView.author = author
                    logView.message = message
                    logView.authorEmail = authorEmail
                    logView.date = date
                    logView.committer = committer
                    logView.committerEmail = committerEmail
                    logView.currentCommit = oid
                    git.currentCommit = oid
                }
            }

            Rectangle {
                anchors.fill: parent
                color: current ? pal.highlight : (index % 2 ? pal.base : pal.alternateBase)
                Text {
                    id: messageText
                    x: 4
                    width: parent.width - 8
//                    wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    elide: Text.ElideRight
                    y: 4
                    text: shortMessage
                    color: current ? pal.highlightedText : pal.text
                }
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (list.currentIndex == index) {
                        logView.details = !logView.details
                    } else {
                        list.currentIndex = index
                        logView.details = true
                    }
                    list.forceActiveFocus()
                }
            }
        }
    }

}
