
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

void Git::setRepoUrl(const QString &url)
{
    m_url = url;
    git_repository_free(m_repo);
    m_repo = 0;
    m_commit.clear();

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

    for (uint i = 0; i < ref_list.count; ++i) {
        const char *refname;
        refname = ref_list.strings[i];
        git_reference *ref;
        git_reference_lookup(&ref, m_repo, refname);

        if (git_reference_is_branch(ref))
            b.append(refname);
    }

    git_strarray_free(&ref_list);

    return b;
}

QString Git::currentBranch() const
{
    return m_branch;
}

void Git::setCurrentBranch(const QString &branch)
{
    m_branch = branch;

    if (m_log)
        m_log->setBranch(branch);

    emit branchesChanged();
}


QString Git::currentCommit() const
{
    return m_commit;
}
void Git::setCurrentCommit(const QString &commit)
{
    qDebug() << "Set commit: " << commit;
    m_commit = commit;
    emit currentCommitChanged();
    m_diffDirty = true;
    emit diffChanged();
}

QAbstractItemModel *Git::logModel()
{
    if (!m_log) {
        m_log = new GitCommitList(m_repo);
        m_log->setBranch(m_branch);
    }

    return m_log;
}

QString Git::diff() const
{
    if (m_diffDirty) {
        m_diff = getDiff();
        m_diffDirty = false;
    }
    return m_diff;
}

QString Git::getDiff() const
{
    if (!m_repo)
        return QString();

    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;

    git_diff_list *diff;

    if (m_commit.isEmpty()) {
        int ret = git_diff_index_to_workdir(&diff, m_repo, NULL, &opts);
        if (ret != 0)
            return QString("Error: Diff failed (%1)").arg(ret);
    } else {
        git_tree *tree1 = 0;
        git_tree *tree2 = 0;

        git_commit *commit;
        git_commit *parent;

        git_oid oid1;
        git_oid_fromstr(&oid1, m_commit.toLatin1().constData());
        git_commit_lookup(&commit, m_repo, &oid1);
        // take the first parent
        git_commit_parent(&parent, commit, 0);

        const git_oid* oid2 = git_commit_id(parent);

        git_object *obj1;
        git_object *obj2;

        git_object_lookup_prefix(&obj1, m_repo, &oid1, m_commit.size(), GIT_OBJ_ANY);
        git_object_lookup_prefix(&obj2, m_repo, oid2, m_commit.size(), GIT_OBJ_ANY);

        git_commit_tree(&tree1, (git_commit *)obj1);
        git_commit_tree(&tree2, (git_commit *)obj2);
        git_object_free(obj1);
        git_object_free(obj2);

        git_diff_tree_to_tree(&diff, m_repo, tree2, tree1, &opts);
        git_tree_free(tree1);
        git_tree_free(tree2);
    }
    QString strDiff;
    int ret = git_diff_print_patch(diff, printer, &strDiff);
    if (ret != 0)
        return QString("Error: Formatting diff failed (%1)").arg(ret);

    git_diff_list_free(diff);

    if (strDiff.isEmpty())
        strDiff = QStringLiteral("No changes in repository.");

    return strDiff;
}

QML_DECLARE_TYPE(Git)
QML_DECLARE_TYPE(QAbstractItemModel)

