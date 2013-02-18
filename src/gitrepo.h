
#ifndef GITREPO_H
#define GITREPO_H

#include <git2.h>
#include <QString>
#include <QDir>
#include <QDebug>

#include "gitcommitlist.h"

class Git : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString repoUrl READ repoUrl WRITE setRepoUrl NOTIFY repoUrlChanged)
    Q_PROPERTY(QString diff READ diff NOTIFY diffChanged)
    Q_PROPERTY(QStringList branches READ branches NOTIFY branchesChanged)
    Q_PROPERTY(QString currentBranch READ currentBranch WRITE setCurrentBranch NOTIFY currentBranchChanged)
    Q_PROPERTY(QAbstractItemModel *logModel READ logModel NOTIFY modelChanged)

public:
    Git();
    ~Git();

    QString repoUrl() const { return m_url; }
    void setRepoUrl(const QString &url);

    QString diff() const;

    bool isValidRepository() const;

    QStringList branches() const;
    QString currentBranch() const;
    void setCurrentBranch(const QString &branch);

    QAbstractItemModel *logModel();

Q_SIGNALS:
    void repoUrlChanged();
    void diffChanged();
    void modelChanged();
    void branchesChanged();
    void currentBranchChanged();

private:
    QString workingDirDiff() const;

    git_repository *m_repo;
    QString m_url;
    QString m_branch;

    mutable QString m_diff;
    mutable bool m_diffDirty;

    GitCommitList *m_log;
};

#endif
