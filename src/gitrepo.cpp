
#include "gitrepo.h"


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

Git::Git()
    : m_repo(0), m_diffDirty(true), m_log(0)
{}

Git::~Git()
{
    delete m_log;
    git_repository_free(m_repo);
}

QString Git::diff() const
{
    if (m_diffDirty) {
        qDebug() << "Getting diff...";
        m_diff = workingDirDiff();
        m_diffDirty = false;
    }
    return m_diff;
}

QString Git::workingDirDiff() const
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

void Git::setRepoUrl(const QString &url)
{
    m_url = url;
    git_repository_free(m_repo);
    m_repo = 0;

    if (QDir::isAbsolutePath(url)) {
        // this probably needs unicode fixing on non-linux FIXME
        if (0 == git_repository_open_ext(&m_repo, url.toUtf8().data(), 0, NULL)) {
            if (m_log)
                m_log->setRepo(m_repo);
            emit repoUrlChanged();
            m_diffDirty = true;
            emit diffChanged();
            emit modelChanged();
            emit branchesChanged();
        }
    }
}

bool Git::isValidRepository() const
{
    return m_repo != 0;
}

QStringList Git::branches() const
{
    if (!m_repo)
        return QStringList();
    QStringList b;

    git_strarray ref_list;
    git_reference_list(&ref_list, m_repo, GIT_REF_LISTALL);

    const char *refname;
    git_reference *ref;

    for (uint i = 0; i < ref_list.count; ++i) {
        refname = ref_list.strings[i];
        git_reference_lookup(&ref, m_repo, refname);

        if (git_reference_is_branch(ref))
            b.append(refname);

//            switch (git_reference_type(ref)) {
//            case GIT_REF_OID:
//                git_oid_fmt(out, git_reference_target(ref));
//                printf("%s [%s]\n", refname, out);
//                break;

//            case GIT_REF_SYMBOLIC:
//                printf("%s => %s\n", refname, git_reference_symbolic_target(ref));
//                break;
//            default:
//                fprintf(stderr, "Unexpected reference type\n");
//                exit(1);
//            }
    }

    git_strarray_free(&ref_list);

    qDebug() << "Branches: " << b;

    return b;
}

QAbstractItemModel *Git::logModel()
{
    if (!m_log)
        m_log = new GitCommitList(m_repo);

    return m_log;
}

QML_DECLARE_TYPE(Git)
QML_DECLARE_TYPE(QAbstractItemModel)

