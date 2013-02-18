

#ifndef GITCOMMITLIST_H
#define GITCOMMITLIST_H

#include <git2.h>

#include "gitrepo.h"

#include <QAbstractListModel>

class GitCommitList : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString repoUrl READ repoUrl WRITE setRepoUrl NOTIFY urlChanged)
    Q_PROPERTY(QString branch READ branch WRITE setBranch NOTIFY branchChanged)

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

    GitCommitList();
    ~GitCommitList();

    QHash<int, QByteArray> roleNames() const;

    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setRepoUrl(const QString &repoUrl);
    QString repoUrl() const;
    void setBranch(const QString &branch);
    QString branch() const;

Q_SIGNALS:
    void urlChanged();
    void branchChanged();

private:
    void loadRepository();
    void loadBranch();

    GitRepo m_repo;
    QString m_branch;

    QVector<git_commit*> m_commits;
};

#endif
