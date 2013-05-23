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


#ifndef GITCACHE_H
#define GITCACHE_H

#include <git2.h>

#include <QObject>
#include <QHash>
#include <QVector>
#include <QList>
#include <QStack>
#include <QString>
#include <QMutex>

class GitCache : public QObject
{
    Q_OBJECT
public:
    GitCache(git_repository *repo);
    ~GitCache();

    QVector<git_commit*> branchData(const QString &branch) { return m_branches.value(branch); }
    QString diff(const QString &commit) {
        if (m_diffs.contains(commit))
            return m_diffs.value(commit);
        return QString();
    }

    // called from main thread to push diff and branches
    void loadBranch(const QString &branch);
    void loadDiff(const QString &commitString);

public slots:
    void doWork();

signals:
    void branchLoaded(QString);
    void diffLoaded(QString);
    void done();
    void statusChanged(QString);


private:
    void processBranch();
    void processDiff();

    QHash<QString, QVector<git_commit*> > m_branches;
    QHash<QString, QString> m_diffs;
    git_repository *m_repo;

    QMutex m_todoMutex;
    QStack<QString> m_branchTodo;
    QStack<QString> m_diffTodo;
};

#endif
