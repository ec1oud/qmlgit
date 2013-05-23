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


#include "gitcommitlist.h"

#include <QDateTime>
#include <QThread>
#include <QDebug>

QHash<int, QByteArray> GitCommitList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(Oid, "oid");
    roles.insert(Author, "author");
    roles.insert(AuthorEmail, "authorEmail");
    roles.insert(Committer, "committer");
    roles.insert(CommitterEmail, "committerEmail");
    roles.insert(Message, "message");
    roles.insert(ShortMessage, "shortMessage");
    roles.insert(ParentCount, "parentCount");
    roles.insert(Time, "time");
    return roles;
}

void GitCommitList::setBranchData(const QVector<git_commit*> &data)
{
    beginResetModel();
    m_commits = data;
    endResetModel();
}

int GitCommitList::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_commits.count();
}

QVariant GitCommitList::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    QVariant ret;
    git_commit *commit = m_commits.at(index.row());

    switch (role) {
    case Message: {
        ret = QString(git_commit_message(commit));
        break;
    }
    case ShortMessage: {
        QString msg = QString(git_commit_message(commit));
        int index = msg.indexOf('\n');
        ret = msg.left(index);
        break;
    }
    case Author: {
        const git_signature *cauth = git_commit_author(commit);
        ret = QString(cauth->name);
        break;
    }
    case AuthorEmail: {
        const git_signature *cauth = git_commit_author(commit);
        ret = QString(cauth->email);
        break;
    }
    case Committer: {
        const git_signature *com = git_commit_author(commit);
        ret = QString(com->name);
        break;
    }
    case CommitterEmail: {
        const git_signature *com = git_commit_author(commit);
        ret = QString(com->email);
        break;
    }
    case ParentCount:
        ret = git_commit_parentcount(commit);
        break;
    case Time: {
        QDateTime time;
        git_time_t timeT = git_commit_time(commit);
        time.setTime_t(timeT);
        ret = QVariant(time);
        break;
    }
    case Oid: {
        const git_oid *oid = git_commit_id(commit);
        char out[41];
        out[40] = '\0';
        git_oid_fmt(out, oid);
        ret = QString(out);
        break;
    }
    }
    return ret;
}
