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


#include <QQuickTextDocument>
#include <QSyntaxHighlighter>

#include <qdebug.h>

class Highlighter : public QSyntaxHighlighter
{
public:
    Highlighter(QTextDocument *parent)
        : QSyntaxHighlighter(parent)
    {
        normal.setFontWeight(QFont::Light);
        normal.setForeground(QColor("#585858"));
        diffRemoved.setFontWeight(QFont::Normal);
        diffRemoved.setForeground(QColor("#D5271E"));
        diffAdded.setFontWeight(QFont::Normal);
        diffAdded.setForeground(Qt::darkGreen);
    }

    void highlightBlock(const QString & text)
    {
        if (text.isEmpty())
            return;

        if (text.at(0) == QLatin1Char('-')) {
            setFormat(0, text.length(), diffRemoved);
        } else if (text.at(0) == QLatin1Char('+')) {
            setFormat(0, text.length(), diffAdded);
        } else {
            setFormat(0, text.length(), normal);
        }
    }
private:
    QTextCharFormat diffAdded;
    QTextCharFormat diffRemoved;
    QTextCharFormat normal;
};

class DiffHighlighter : public QQuickItem
{
    Q_OBJECT

    Q_PROPERTY(QQuickItem *target READ target WRITE setTarget NOTIFY targetChanged)

public:
    DiffHighlighter() : m_highlighter(0)
    {}

    QQuickItem *target() { return m_target; }

    void setTarget(QQuickItem *target)
    {
        m_target = target;
        emit targetChanged();

        QVariant doc = m_target->property("textDocument");
        if (doc.canConvert<QQuickTextDocument*>()) {
            QQuickTextDocument *qqDoc = doc.value<QQuickTextDocument*>();

            if (qqDoc) {
                if (m_highlighter)
                    delete m_highlighter;
                m_highlighter = new Highlighter(qqDoc->textDocument());
            }
        }
    }

Q_SIGNALS:
    void targetChanged();

private:
    QQuickItem *m_target;
    QSyntaxHighlighter *m_highlighter;
};
