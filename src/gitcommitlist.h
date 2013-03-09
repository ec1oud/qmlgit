

#ifndef GITCOMMITLIST_H
#define GITCOMMITLIST_H

#include <git2.h>
#include <QAbstractListModel>

#include "gitcache.h"

class GitCommitList : public QAbstractListModel
{
    Q_OBJECT

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

    virtual QHash<int, QByteArray> roleNames() const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setBranchData(const QVector<git_commit*> &data);

private:
    void update();
    void loadBranch();
    void clear();

    git_repository *m_repo;
    QVector<git_commit*> m_commits;
};

#endif
