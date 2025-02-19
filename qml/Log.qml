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

Item {
    anchors.fill: parent

    GitModel {
        id: logModel
        git: gitRepo
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        RowLayout {
            width: parent.width
            height: 40
            spacing: 4

            Label {
                text: "Branch:"
            }
            ComboBox {
                id: comboBox
                model: gitRepo.branches
                implicitWidth: 200
                onCurrentTextChanged: logModel.currentBranch = currentText
            }
            Label {
                id: authorLabel
                text: (logView.author.length > 0) ? logView.author.concat(" <", logView.authorEmail, ">") : ""
                horizontalAlignment: Text.AlignRight
                Layout.fillWidth: true
            }
        }

        Row {
            height: parent.height - 40
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 4

            LogView {
                id: logView
                gitModel: logModel
                width: parent.width / 3
                height: parent.height
            }
            SplitView {
                id: splitter
                orientation: Qt.Vertical
                width: parent.width / 3 * 2
                height: parent.height
                ScrollView {
//                    frameVisible: true
                    id: commitView
                    width: parent.width
                    height: 120
                    TextArea {
                        x: 0
                        y: 0
                        width: splitter.width
                        text: logView.message.concat("\n", logView.date).concat(logView.currentCommit)
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        selectByKeyboard: true
                    }
                }
                DiffView {
                    width: parent.width
                    height: 120
                    text: logModel.diff
                }
            }
        }
    }
}
