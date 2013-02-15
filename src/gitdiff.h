


#ifndef GITDIFF_H
#define GITDIFF_H


#include <QObject>

#include <git2.h>

class GitDiff : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString diff READ diff NOTIFY diffChanged)

public:
    GitDiff();
    ~GitDiff();

    QString diff() const;
    void setRepo(git_repository * repo);

Q_SIGNALS:
    void diffChanged();

private:
    QString workingDirDiff() const;

    QString m_repoUrl;
    mutable QString m_diff;
    mutable bool m_diffDirty;

    git_repository *m_repo;
};

#endif
