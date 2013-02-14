


#ifndef GITDIFF_H
#define GITDIFF_H

#include <QObject>


class GitDiff : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString repo READ repo WRITE setRepo NOTIFY repoChanged)
    Q_PROPERTY(QString diff READ diff FINAL)

public:
    GitDiff();

    QString diff() { return m_diff; }

    QString repo() { return m_repo; }
    void setRepo(const QString &repo);

Q_SIGNALS:
    void repoChanged(QString);

private:
    void refresh();

    QString m_diff;
    QString m_repo;
};

#endif

