


#ifndef GITDIFF_H
#define GITDIFF_H

#include <QObject>

#include <git2.h>

class GitStuff : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString repo READ repo WRITE setRepo NOTIFY repoChanged)
    Q_PROPERTY(QString diff READ diff NOTIFY diffChanged)

public:
    GitStuff();
    ~GitStuff();

    QString diff() const;
    QString repo() { return m_repoUrl; }
    void setRepo(const QString &repo);

Q_SIGNALS:
    void repoChanged(QString);
    void diffChanged();

private:
    QString workingDirDiff() const;

    QString m_repoUrl;
    mutable QString m_diff;
    mutable bool m_diffDirty;

    git_repository *m_repo;
};

#endif

