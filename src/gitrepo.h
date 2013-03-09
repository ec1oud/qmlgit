
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
    Q_PROPERTY(QString currentCommit READ currentCommit WRITE setCurrentCommit NOTIFY currentCommitChanged)
    Q_PROPERTY(QAbstractItemModel *logModel READ logModel NOTIFY modelChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

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

    QString currentCommit() const;
    void setCurrentCommit(const QString &commit);

    QString statusMessage() const { return m_statusMessage; }

    QAbstractItemModel *logModel();

public slots:
    void branchLoaded(const QString &branch);
    void diffLoaded(const QString &commit);

signals:
    void repoUrlChanged();
    void diffChanged();
    void modelChanged();
    void branchesChanged();
    void currentBranchChanged();
    void currentCommitChanged();
    void statusMessageChanged();

    void loadBranch(QString);
    void loadDiff(QString);

private:
    void setStatusMessage(const QString &status);
    QString getDiff() const;
    void clearLog();

    GitCache *m_gitCache;
    QThread *m_workerThread;
    git_repository *m_repo;
    QString m_url;
    QString m_branch;
    QString m_commit;
    QString m_statusMessage;

    mutable QString m_diff;
    mutable bool m_diffDirty;

    GitCommitList *m_log;

};

#endif
