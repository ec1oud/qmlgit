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


#include <QtTest/QtTest>

#include <qgit2.h>

class tst_gitCache : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void tags();
    void references();

private:
    LibQGit2::Repository repo;
};

void tst_gitCache::initTestCase()
{
    LibQGit2::initLibQGit2();
    QString path = QStringLiteral(".");
    repo.discoverAndOpen(path);
}

void tst_gitCache::cleanupTestCase()
{
}

void tst_gitCache::tags()
{
    QVERIFY(!repo.isBare());
    qDebug() << repo.head().name();
    QStringList tags = repo.listTags();
    qDebug() << tags;


//     git_strarray tag_names;
//     QCOMPARE(git_tag_list(&tag_names, repo), 0);
//     for (uint i = 0; i < tag_names.count; ++i)
//         qDebug() << "Found TAG: " << tag_names.strings[i];
//
//     git_strarray_free(&tag_names);
}

void tst_gitCache::references()
{
//    GIT_REF_INVALID
//    GIT_REF_OID        points to OID
//    GIT_REF_SYMBOLIC   points to reference
//    GIT_REF_PACKED
//    GIT_REF_HAS_PEEL
//    GIT_REF_LISTALL

    auto refs = repo.listReferences();
    foreach(auto &ref, refs) {
        qDebug() << "REF: " << ref;
    }
}



QTEST_MAIN(tst_gitCache)
#include "tst_gitcache.moc"

