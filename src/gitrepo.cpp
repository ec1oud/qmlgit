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


#include "gitrepo.h"


#include "gitdiff.h"

#include <git2.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QObject>
#include <QString>
#include <QDir>
#include <QThread>

#include <QDebug>
#include <QQuickItem>

using namespace LibQGit2;

Git::Git()
    : m_workerThread(0), m_diffDirty(true), m_gitCache(0), m_log(new GitCommitList())
{
    m_workerThread = new QThread(this);
    qDebug() << "main thread: " << thread()->currentThreadId();
}

Git::~Git()
{
    delete m_gitCache;
    delete m_log;
}

void Git::setRepoUrl(const QString &url)
{
    m_url = url;
    m_commit.clear();
    m_branch.clear();

    try {
        m_repository.discoverAndOpen(url);
    } catch (const LibQGit2::Exception& ex) {
        qDebug() << ex.what();
        return;
    }

    if (m_gitCache) {
        m_workerThread->quit();
        m_gitCache->deleteLater();
    }
    m_gitCache = 0;

    m_log->setBranchData(QVector<Commit>());

    if (QDir::isAbsolutePath(url)) {
        m_diffDirty = true;
        m_gitCache = new GitCache(m_repository);
        connect(this, &Git::updateCache, m_gitCache, &GitCache::doWork);

        connect(m_gitCache, &GitCache::branchLoaded, this, &Git::branchLoaded);
        connect(m_gitCache, &GitCache::diffLoaded, this, &Git::diffLoaded);
        m_gitCache->moveToThread(m_workerThread);

        connect(m_gitCache, &GitCache::done, m_workerThread, &QThread::quit);
        connect(m_gitCache, &GitCache::statusChanged, this, &Git::setStatusMessage);
        m_workerThread->start();
    }
    emit repoUrlChanged();
    emit diffChanged();
    emit modelChanged();
    emit branchesChanged();
}

bool Git::isValidRepository() const
{
    return true; // m_repository.status() != 0;
}

QStringList Git::branches()
{
    QStringList b;
    QVector<Reference> refs = m_repository.references();
    foreach(const Reference &ref, refs) {
        if (ref.isBranch())
            b.append(ref.name());
    }
    return b;
}

QString Git::currentBranch() const
{
    return m_branch;
}

void Git::setCurrentBranch(const QString &branch)
{
    m_branch = branch;
    if (!branch.isEmpty()) {
        Q_ASSERT(m_gitCache);
        m_gitCache->loadBranch(branch);
        emit updateCache();
    }
}

void Git::branchLoaded(const QString &branch)
{
    if (branch == m_branch) {
        m_log->setBranchData(m_gitCache->branchData(branch));
        emit modelChanged();
    }
}

void Git::diffLoaded(const QString &commit)
{
    if (commit == m_commit)
        emit diffChanged();
}

QString Git::currentCommit() const
{
    return m_commit;
}

void Git::setCurrentCommit(const QString &commit)
{
    m_commit = commit;
    if (!m_commit.isEmpty()) {
        m_gitCache->loadDiff(commit);
        emit updateCache();
    }
    emit diffChanged();
}

QAbstractItemModel *Git::logModel()
{
    return m_log;
}

QString Git::diff() const
{
    if (!m_commit.isEmpty())
        return m_gitCache->diff(m_commit);
    return QString();
}

void Git::setStatusMessage(const QString &status)
{
    m_statusMessage = status;
    emit statusMessageChanged();
}

QML_DECLARE_TYPE(Git)
QML_DECLARE_TYPE(QAbstractItemModel)
