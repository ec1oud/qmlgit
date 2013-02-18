
#ifndef GITREPO_H
#define GITREPO_H

#include <git2.h>
#include <QString>
#include <QDir>

class GitRepo
{
public:
    GitRepo() : m_repo(0) {}
    ~GitRepo()
    {
        git_repository_free(m_repo);
    }

    bool open(const QString &url)
    {
        m_url = url;
        git_repository_free(m_repo);
        m_repo = 0;

        if (QDir::isAbsolutePath(url)) {
            // this probably needs unicode fixing on non-linux FIXME
            return 0 == git_repository_open_ext(&m_repo, url.toUtf8().data(), 0, NULL);
        }
        return false;
    }

    QString url() const { return m_url; }

    git_repository *git_repo() const
    {
        return m_repo;
    }


    bool isValid() const
    {
        return m_repo != 0;
    }

private:
    git_repository *m_repo;
    QString m_url;
};

#endif
