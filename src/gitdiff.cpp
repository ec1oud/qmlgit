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

//static int printer(
//  const git_diff_delta */*delta*/,
//  const git_diff_range */*range*/,
//  char /*usage*/,
//  const char *line,
//  size_t /*line_len*/,
//  void *data)
//{
//  QString *strDiff = static_cast<QString*>(data);

////    switch (usage) {
////    case GIT_DIFF_LINE_ADDITION: color = 3; break;
////    case GIT_DIFF_LINE_DELETION: color = 2; break;
////    case GIT_DIFF_LINE_ADD_EOFNL: color = 3; break;
////    case GIT_DIFF_LINE_DEL_EOFNL: color = 2; break;
////    case GIT_DIFF_LINE_FILE_HDR: color = 1; break;
////    case GIT_DIFF_LINE_HUNK_HDR: color = 4; break;
////    default: color = 0;
////    }

//  strDiff->append(line);
//  return 0;
//}

GitDiff::GitDiff()
    : m_diffDirty(true)
{
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

//QString GitDiff::repoUrl() const
//{
//    return m_repo.url();
//}

QString GitDiff::workingDirDiff() const
{
    if (!m_repo.isValidRepository())
        return QString();

//    git_diff_options opts = GIT_DIFF_OPTIONS_INIT;

//    git_diff_list *diff;

//    int ret = git_diff_index_to_workdir(&diff, m_repo.git_repo(), NULL, &opts);
//    if (ret != 0)
//        return QString("Error: Diff failed (%1)").arg(ret);

    QString strDiff;
//    ret = git_diff_print_patch(diff, printer, &strDiff);
//    if (ret != 0)
//        return QString("Error: Formatting diff failed (%1)").arg(ret);

//    git_diff_list_free(diff);

//    if (strDiff.isEmpty())
//        strDiff = QStringLiteral("No changes in repository.");


    return strDiff;
}

QML_DECLARE_TYPE(GitDiff)
