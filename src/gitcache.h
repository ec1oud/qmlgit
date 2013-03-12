
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
