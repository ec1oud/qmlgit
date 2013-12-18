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


#ifndef GITREPO_H
#define GITREPO_H

#include <QString>
#include <QDir>
#include <QDebug>

#include <qgit2.h>

#include "gitcache.h"

class Git : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString repoUrl READ repoUrl WRITE setRepoUrl NOTIFY repoUrlChanged)
    Q_PROPERTY(QStringList branches READ branches NOTIFY branchesChanged)
    Q_PROPERTY(QString statusMessage READ statusMessage NOTIFY statusMessageChanged)

public:
    Git();
    ~Git();

    QString repoUrl() const { return m_url; }
    void setRepoUrl(const QString &url);

    bool isValidRepository() const;

    QStringList branches();

    GitCache *cache() const {
        return m_gitCache;
    }

    QString statusMessage() const { return m_statusMessage; }

signals:
    void repoUrlChanged();
    void branchesChanged();
    void statusMessageChanged();
    void updateCache();

private:
    void setStatusMessage(const QString &status);
    QString getDiff() const;
    void clearLog();
    
    QThread *m_workerThread;
    QString m_url;
    QString m_statusMessage;

    LibQGit2::Repository m_repository;
    GitCache *m_gitCache;
};

#endif
