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

#include <qgit2.h>

using namespace LibQGit2;

GitCache::GitCache(const Repository &repo)
    : m_repo(repo)
{
}

GitCache::~GitCache()
{
    m_branches.clear();
}

void GitCache::loadBranch(const QString &branch)
{
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

    Reference ref = m_repo.lookupRef(branch);
    RevWalk walk(m_repo);
    walk.push(ref);

    int count = 0;
    Commit commit;

    while (walk.next(commit)) {
        m_branches[branch].append(commit);
        ++count;
        if (count % 100 == 0) {
            emit statusChanged(QString("%1 commits loaded...").arg(count));
        }
    }

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

     Diff diff;
     if (commitString.isEmpty()) {
         diff = m_repo.diffIndexToWorkdir();
     } else {
         OId oid;
         oid.fromString(commitString);
         Commit commit = m_repo.lookupCommit(oid);
         if (commit.parentCount() == 0) // initial commit
             diff = m_repo.diffTrees(Tree(), commit.tree());
         else
             diff = m_repo.diffTrees(commit.parent(0).tree(), commit.tree());
         // FIXME handle more than one parent
     }

     QString strDiff = diff.toPlainText();
     if (strDiff.isEmpty())
         strDiff = QStringLiteral("No changes in repository.");

     m_diffs[commitString] = strDiff;
     emit diffLoaded(commitString);
     emit statusChanged(QString());
}
