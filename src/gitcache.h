
#ifndef GITCACHE_H
#define GITCACHE_H

#include <git2.h>

#include <QObject>
#include <QHash>
#include <QVector>
#include <QList>
#include <QString>

class GitCache : public QObject
{
    Q_OBJECT
public:
    GitCache(git_repository *repo) :m_repo(repo) { Q_ASSERT(repo); }
    ~GitCache();


    QVector<git_commit*> branchData(const QString &branch) { return m_branches.value(branch); }
    QString diff(const QString &commit) {
        if (m_diffs.contains(commit))
            return m_diffs.value(commit);
        return QString();
    }

public slots:
    void loadBranch(const QString &branch);
    void loadDiff(const QString &commitString);

signals:
    void branchLoaded(QString);
    void diffLoaded(QString);
    void done();
    void statusChanged(QString);

private:
    void processBranches();
    void processDiffs();

    QHash<QString, QVector<git_commit*> > m_branches;
    QHash<QString, QString> m_diffs;
    git_repository *m_repo;
};

#endif
