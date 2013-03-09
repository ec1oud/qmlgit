
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
