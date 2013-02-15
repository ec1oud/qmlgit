#include "gitdiff.h"

#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QObject>
#include <QString>
#include <QDir>

#include <QDebug>

static int printer(
  const git_diff_delta */*delta*/,
  const git_diff_range */*range*/,
  char /*usage*/,
  const char *line,
  size_t /*line_len*/,
  void *data)
{
  QString *strDiff = static_cast<QString*>(data);

//    switch (usage) {
//    case GIT_DIFF_LINE_ADDITION: color = 3; break;
//    case GIT_DIFF_LINE_DELETION: color = 2; break;
//    case GIT_DIFF_LINE_ADD_EOFNL: color = 3; break;
//    case GIT_DIFF_LINE_DEL_EOFNL: color = 2; break;
//    case GIT_DIFF_LINE_FILE_HDR: color = 1; break;
//    case GIT_DIFF_LINE_HUNK_HDR: color = 4; break;
//    default: color = 0;
//    }

  strDiff->append(line);
  return 0;
}

GitStuff::GitStuff()
    : m_repo(0), m_diffDirty(true)
{}

GitStuff::~GitStuff()
{
    git_repository_free(m_repo);
}

void GitStuff::setRepo(const QString &repo)
{
    git_repository_free(m_repo);
    m_repo = 0;

    if (QDir::isAbsolutePath(repo)) {
        qDebug() << "Loading repository...";

        // this probably needs unicode fixing on non-linux FIXME
        int open = git_repository_open_ext(&m_repo, repo.toUtf8().data(), 0, NULL);
        if (open == 0) {
            m_repoUrl = repo;
            m_diffDirty = true;

            emit repoChanged(m_repoUrl);
            emit diffChanged();
            return;
        }
    }
    m_diffDirty = false;
    m_diff = QStringLiteral("Could not open repository.");
    m_repoUrl = QString();
    emit repoChanged(m_repoUrl);
}

QString GitStuff::diff() const
{
    if (m_diffDirty) {
        qDebug() << "Getting diff...";
        m_diff = workingDirDiff();
        m_diffDirty = false;
    }
    return m_diff;
}

QString GitStuff::workingDirDiff() const
{
    if (!m_repo)
        return QString();

    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;

    git_diff_list *diff;

    int ret = git_diff_index_to_workdir(&diff, m_repo, NULL, &opts);
    if (ret != 0)
        return QString("Error: Diff failed (%1)").arg(ret);

    QString strDiff;
    ret = git_diff_print_patch(diff, printer, &strDiff);
    if (ret != 0)
        return QString("Error: Formatting diff failed (%1)").arg(ret);

    git_diff_list_free(diff);

    if (strDiff.isEmpty())
        strDiff = QStringLiteral("No changes in repository.");


    return strDiff;
}

