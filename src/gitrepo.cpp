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
    : m_workerThread(0), m_gitCache(0)
{
    m_workerThread = new QThread(this);
    qDebug() << "main thread: " << thread()->currentThreadId();
}

Git::~Git()
{
    delete m_gitCache;
}

void Git::setRepoUrl(const QString &url)
{
    m_url = url;

    try {
        m_repository.discoverAndOpen(url);
    } catch (const LibQGit2::Exception& ex) {
        qDebug() << ex.what();
        return;
    }

    if (m_gitCache) {
//        m_workerThread->quit();
        m_gitCache->deleteLater();
    }
    m_gitCache = 0;

    if (QDir::isAbsolutePath(url)) {
        m_gitCache = new GitCache(m_repository);
        m_gitCache->moveToThread(m_workerThread);
//        connect(m_gitCache, &GitCache::done, m_workerThread, &QThread::quit);
        connect(m_gitCache, &GitCache::statusChanged, this, &Git::setStatusMessage);
        m_workerThread->start();
    }
    emit repoUrlChanged();
    emit branchesChanged();
}

bool Git::isValidRepository() const
{
    return true; // m_repository.status() != 0;
}

QStringList Git::branches()
{
    QStringList b;
    if (!m_repository.constData())
        return b;

    QVector<Reference> refs = m_repository.references();
    foreach(const Reference &ref, refs) {
        if (ref.isBranch())
            b.append(ref.name());
    }
    return b;
}

void Git::setStatusMessage(const QString &status)
{
    m_statusMessage = status;
    emit statusMessageChanged();
}


QML_DECLARE_TYPE(Git)
QML_DECLARE_TYPE(QAbstractItemModel)
