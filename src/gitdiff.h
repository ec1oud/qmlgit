


#ifndef GITDIFF_H
#define GITDIFF_H


#include <QObject>

#include <git2.h>

#include "gitrepo.h"

class GitDiff : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString repoUrl READ repoUrl WRITE setRepoUrl NOTIFY urlChanged)
    Q_PROPERTY(QString diff READ diff NOTIFY diffChanged)

public:
    GitDiff();
    ~GitDiff();

    QString diff() const;
    void setRepoUrl(const QString &repoUrl);
    QString repoUrl() const;

Q_SIGNALS:
    void urlChanged();
    void diffChanged();

private:
    QString workingDirDiff() const;

    QString m_repoUrl;
    mutable QString m_diff;
    mutable bool m_diffDirty;

    GitRepo m_repo;
};

#endif
