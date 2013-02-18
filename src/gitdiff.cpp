#include "gitdiff.h"

#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QObject>
#include <QString>
#include <QDir>

#include <QDebug>
#include <QQuickItem>

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

GitDiff::GitDiff()
    : m_diffDirty(true)
{
}

void GitDiff::setRepoUrl(const QString &url)
{
    m_repo.open(url);

    if (m_repo.isValid()) {
        qDebug() << "Loading repository...";
        m_diffDirty = true;
    } else {
        m_diff = QStringLiteral("Could not open repository.");
        m_diffDirty = false;
    }
    emit urlChanged();
    emit diffChanged();
}

QString GitDiff::diff() const
{
    if (m_diffDirty) {
        qDebug() << "Getting diff...";
        m_diff = workingDirDiff();
        m_diffDirty = false;
    }
    return m_diff;
}

QString GitDiff::repoUrl() const
{
    return m_repo.url();
}

QString GitDiff::workingDirDiff() const
{
    if (!m_repo.isValid())
        return QString();

    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;

    git_diff_list *diff;

    int ret = git_diff_index_to_workdir(&diff, m_repo.git_repo(), NULL, &opts);
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

QML_DECLARE_TYPE(GitDiff)
