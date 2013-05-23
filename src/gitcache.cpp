/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of qmlgit.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
****************************************************************************/


#include "gitcache.h"

#include <QDateTime>
#include <QThread>
#include <QDebug>
#include <QtTest/qtest.h>


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

GitCache::GitCache(git_repository *repo)
    : m_repo(repo)
{
    Q_ASSERT(repo);
}

GitCache::~GitCache()
{
    foreach (const QString &branch, m_branches.keys())
        foreach (git_commit *commit, m_branches[branch])
            git_commit_free(commit);
    m_branches.clear();
}

void GitCache::loadBranch(const QString &branch)
{
    Q_ASSERT(m_repo);
    Q_ASSERT(!branch.isEmpty());

    if (m_branches.contains(branch)) {
        emit branchLoaded(branch);
        return;
    }
    QMutexLocker locker(&m_todoMutex);
    m_branchTodo.push(branch);
}

void GitCache::loadDiff(const QString &commitString)
{
    if (!m_repo)
        return;

    if (m_diffs.contains(commitString)) {
        emit diffLoaded(commitString);
        return;
    }

    QMutexLocker locker(&m_todoMutex);
    m_diffTodo.push(commitString);
}

void GitCache::doWork()
{
    processDiff();
    processBranch();
}

void GitCache::processBranch()
{
    QMutexLocker locker(&m_todoMutex);
    if (m_branchTodo.isEmpty())
        return;
    QString branch = m_branchTodo.pop();
    locker.unlock();

    if (m_branches.contains(branch)) {
        emit branchLoaded(branch);
        return;
    }
    emit statusChanged(QStringLiteral("Loading branch..."));

    git_reference *ref;
    int error = git_reference_lookup(&ref, m_repo, branch.toUtf8().constData());
    if (error != 0) {
        qWarning() << "Looking up reference failed: " + branch;
        return;
    }

    git_oid oid;
    error = git_reference_name_to_id(&oid, m_repo, branch.toUtf8().constData());
    Q_ASSERT(!error);
    git_reference_free(ref);

    git_revwalk *walk;
    git_commit *wcommit;

    git_revwalk_new(&walk, m_repo);
    error = git_revwalk_push(walk, &oid);
    Q_ASSERT(error == 0);

    int count = 0;
    while (git_revwalk_next(&oid, walk) == 0) {
        error = git_commit_lookup(&wcommit, m_repo, &oid);
        if (!error) {
            m_branches[branch].append(wcommit);
        } else {
            qWarning() << "Error looking up commit!";
        }
        ++count;
        if (count % 100 == 0)
            emit statusChanged(QString("%1 commits loaded...").arg(count));
    }

    git_revwalk_free(walk);
    emit branchLoaded(branch);
    emit statusChanged(QString());
}

void GitCache::processDiff()
{
    QMutexLocker locker(&m_todoMutex);
    if (m_diffTodo.isEmpty())
        return;

    QString commitString = m_diffTodo.pop();
    locker.unlock();

    if (m_diffs.contains(commitString)) {
        emit diffLoaded(commitString);
        return;
    }

    emit statusChanged(QStringLiteral("Loading diff..."));

    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;
    git_diff_list *diff;

    if (commitString.isEmpty()) {
        int ret = git_diff_index_to_workdir(&diff, m_repo, NULL, &opts);
        if (ret != 0) {
            m_diffs[commitString] = QString("Error: Diff failed (%1)").arg(ret);
            return;
        }
    } else {
        git_tree *tree1 = 0;
        git_tree *tree2 = 0;

        git_commit *commit;
        git_commit *parent;

        git_oid oid1;
        git_oid_fromstr(&oid1, commitString.toLatin1().constData());
        git_commit_lookup(&commit, m_repo, &oid1);
        // take the first parent
        git_commit_parent(&parent, commit, 0);

        const git_oid* oid2 = git_commit_id(parent);

        git_object *obj1;
        git_object *obj2;

        git_object_lookup_prefix(&obj1, m_repo, &oid1, commitString.size(), GIT_OBJ_ANY);
        git_object_lookup_prefix(&obj2, m_repo, oid2, commitString.size(), GIT_OBJ_ANY);

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
    if (ret != 0) {
        m_diffs[commitString] = QString("Error: Formatting diff failed (%1)").arg(ret);
        return;
    }

    git_diff_list_free(diff);

    if (strDiff.isEmpty())
        strDiff = QStringLiteral("No changes in repository.");

    m_diffs[commitString] = strDiff;
    emit diffLoaded(commitString);
    emit statusChanged(QString());
}
