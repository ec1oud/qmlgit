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
import QtQuick.Controls 1.1
import QtQuick.Layouts 1.1
import Git 1.0



Item {
    property alias branch: gitModel.currentBranch

    SystemPalette { id: pal }
    GitModel {
        id: gitModel
        git: gitRepo
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: 4
        frameVisible: true

        ListView {
            id: list
            anchors.fill: parent
            model: gitModel
            delegate: commitDelegate

            onContentYChanged: canvas.requestPaint()

            Canvas {
                id: canvas
                anchors.fill: parent

                property int horizontalDistance: 16
                property int lineHeight: 20
                property int blobSize: 10

                function lineSection(ctx, x, y) {
                    ctx.beginPath();
                    ctx.moveTo(x, y);
                    ctx.lineTo(x, y + lineHeight);
                    ctx.stroke();
                }
                function lineLeft(ctx, x, y) {
                    ctx.beginPath();
                    ctx.moveTo(x, y);
                    ctx.bezierCurveTo(x, y + lineHeight*.75, x - horizontalDistance, y + lineHeight*.25, x - horizontalDistance, y + lineHeight);
                    ctx.stroke();
                }
                function lineRight(ctx, x, y) {
                    ctx.beginPath();
                    ctx.moveTo(x, y);
                    ctx.bezierCurveTo(x, y + lineHeight*.75, x + horizontalDistance, y + lineHeight*.25, x + horizontalDistance, y + lineHeight);
                    ctx.stroke();
                }
                function blob(ctx, x, y) {
                    ctx.ellipse(x -blobSize/2, y -blobSize/2 + lineHeight/2, blobSize, blobSize);
                    ctx.fill();
                }

                onPaint: {
                    var ctx = canvas.getContext('2d');

                    ctx.clearRect(0,0,canvas.width, canvas.height);

                    ctx.fillStyle = "#008080"
                    ctx.strokeStyle = "#008080"

//                    lineSection(ctx, 1*horizontalDistance, 0*lineHeight)
//                    lineSection(ctx, 1*horizontalDistance, 1*lineHeight)
//                    lineSection(ctx, 1*horizontalDistance, 2*lineHeight)

//                    lineRight(ctx, 1*horizontalDistance, 0)
//                    lineSection(ctx, 2*horizontalDistance, 1*lineHeight)
//                    lineSection(ctx, 2*horizontalDistance, 2*lineHeight)
//                    blob(ctx, 2*horizontalDistance, 2*lineHeight)
//                    lineLeft(ctx, 2*horizontalDistance, 2*lineHeight)

                    var yOffset = list.contentY % lineHeight

                    for (var i = 0; i <= list.height / lineHeight; i++) {
                        lineSection(ctx, 1*horizontalDistance, i*lineHeight - yOffset)
                        blob(ctx, 1*horizontalDistance, i*lineHeight - yOffset)
                    }
                }
            }
        }
    }

    Component {
        id: commitDelegate

        Item {
            width: parent.width
            height: canvas.lineHeight //messageText.height + 1

            property bool current: ListView.isCurrentItem
            onCurrentChanged: {
                if (current) {
                    gitModel.currentCommit = oid
                }
            }

            Rectangle {
                anchors.fill: parent
                color: current ? pal.highlight : (index % 2 ? pal.base : pal.alternateBase)
                Text {
                    id: messageText
                    text: shortMessage

                    x: 4 + 4 * canvas.horizontalDistance
                    width: parent.width - 8
                    elide: Text.ElideRight
                    color: current ? pal.highlightedText : pal.text
                }
            }

            //            MouseArea {
            //                anchors.fill: parent
            //                onClicked: {
            //                    list.currentIndex = index
            //                    list.forceActiveFocus()
            //                }
            //            }
        }
    }
}
