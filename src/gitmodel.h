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



#ifndef GITCOMMITLIST_H
#define GITCOMMITLIST_H

#include <qgit2.h>
#include <QAbstractListModel>

#include "gitrepo.h"
#include "gitcache.h"

class GitModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(Git *git READ git WRITE setGit NOTIFY gitChanged)

    Q_PROPERTY(QString currentBranch READ currentBranch WRITE setCurrentBranch NOTIFY currentBranchChanged)
    Q_PROPERTY(QString currentCommit READ currentCommit WRITE setCurrentCommit NOTIFY currentCommitChanged)
    Q_PROPERTY(QString diff READ diff NOTIFY diffChanged)

public:
    enum Roles {
        Oid = Qt::UserRole + 1,
        Author,
        AuthorEmail,
        Committer,
        CommitterEmail,
        Message,
        ShortMessage,
        ParentCount,
        Time
    };

    GitModel();

    Git *git() const { return m_repo; }
    void setGit(Git *repo);

    virtual QHash<int, QByteArray> roleNames() const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setBranchData(const QVector<LibQGit2::Commit> &data);

    QString diff() const;

    QStringList branches() { return m_repo->branches(); }
    QString currentBranch() const;
    void setCurrentBranch(const QString &branch);

    QString currentCommit() const;
    void setCurrentCommit(const QString &commit);

    Q_INVOKABLE void startDrag(const QString &oid);

public slots:
    void branchLoaded(const QString &branch);
    void diffLoaded(const QString &commit);

    void repoUrlChanged();

signals:
    void gitChanged();
    void diffChanged();
    void modelChanged();
    void branchesChanged();
    void currentBranchChanged();
    void currentCommitChanged();

    void updateCache();

private:
    void update();
    void loadBranch();
    void clear();

    Git *m_repo;
    QVector<LibQGit2::Commit> m_commits;
    QString m_branch;
    QString m_commit;

    mutable QString m_diff;
    mutable bool m_diffDirty;
};

#endif
