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

#include <git2.h>

class tst_gitCache : public QObject
{
    Q_OBJECT
private slots:
    void initTestCase();
    void cleanupTestCase();

    void tags();
    void references();

private:
    git_repository *repo;
};

void tst_gitCache::initTestCase()
{
    const char *path = "/home/frederik/dev/qt-src-dev/qtbase";
    QCOMPARE(git_repository_open(&repo, path), 0);
}

void tst_gitCache::cleanupTestCase()
{
    git_repository_free(repo);
}

void tst_gitCache::tags()
{
    git_strarray tag_names;
    QCOMPARE(git_tag_list(&tag_names, repo), 0);
    for (uint i = 0; i < tag_names.count; ++i)
        qDebug() << "Found TAG: " << tag_names.strings[i];

    git_strarray_free(&tag_names);
}

void tst_gitCache::references()
{
//    GIT_REF_INVALID
//    GIT_REF_OID        points to OID
//    GIT_REF_SYMBOLIC   points to reference
//    GIT_REF_PACKED
//    GIT_REF_HAS_PEEL
//    GIT_REF_LISTALL

    git_strarray references;
    QCOMPARE(git_reference_list(&references, repo, GIT_REF_OID), 0);
    for (uint i = 0; i < references.count; ++i) {
        qDebug() << "Found REFERENCE: " << references.strings[i];
    }
    git_strarray_free(&references);

    QCOMPARE(git_reference_list(&references, repo, GIT_REF_SYMBOLIC), 0);
    for (uint i = 0; i < references.count; ++i)
        qDebug() << "Found SYMBOLIC REF: " << references.strings[i];
    git_strarray_free(&references);

    QCOMPARE(git_reference_list(&references, repo, GIT_REF_PACKED), 0);
    for (uint i = 0; i < references.count; ++i)
        qDebug() << "Found PACKED REF: " << references.strings[i];
    git_strarray_free(&references);

    QCOMPARE(git_reference_list(&references, repo, GIT_REF_HAS_PEEL), 0);
    for (uint i = 0; i < references.count; ++i)
        qDebug() << "Found REF_HAS_PEEL: " << references.strings[i];
    git_strarray_free(&references);


    QCOMPARE(git_reference_list(&references, repo, GIT_REF_LISTALL), 0);
    for (uint i = 0; i < references.count; ++i) {
        qDebug() << "REFERENCE: " << references.strings[i];
        git_reference *ref;
        QCOMPARE(git_reference_lookup(&ref, repo, references.strings[i]), 0);
        qDebug() << "     type: " << (git_reference_type(ref) & GIT_REF_OID ? "oid" : "not oid") << "; "
                    << (git_reference_type(ref) & GIT_REF_SYMBOLIC ? "symbolic" : "not sym") << "; "
                    << (git_reference_type(ref) & GIT_REF_PACKED ? "packed" : "not packed")
                    << "; is branch:" << git_reference_is_branch(ref)
                    << "; is remote:" << git_reference_is_remote(ref)
                       ;
//        char out[80];
//        git_reference_normalize_name(out, 80, references.strings[i], GIT_REF_FORMAT_ALLOW_ONELEVEL);
//        qDebug() << "     normalized GIT_REF_FORMAT_ALLOW_ONELEVEL: " << out;

//        git_reference_normalize_name(out, 80, references.strings[i], GIT_REF_FORMAT_REFSPEC_PATTERN);
//        qDebug() << "     normalized GIT_REF_FORMAT_ALLOW_ONELEVEL: " << out;
//        // git_reference_peel gives the target object??


    }
    git_strarray_free(&references);
}



class GitReference
{
public:
    GitReference(git_reference *ref) : m_ref(ref)
    {}
    // todo: copy and assignment ctor...

    bool isBranch() const;
    bool isTag() const;
    bool isRemote() const;
    QString name() const; // some form of pretty name
    git_oid oid() const;

private:
    git_reference *m_ref;
};

bool GitReference::isBranch() const
{
    return git_reference_is_branch(m_ref);
}

bool GitReference::isTag() const
{
    // fixme
    return false;
}

bool GitReference::isRemote() const
{
    return git_reference_is_remote(m_ref);
}

QString GitReference::name() const
{
    return QString::fromUtf8(git_reference_name(m_ref));
}

git_oid GitReference::oid() const
{
    return git_oid();
}



QTEST_MAIN(tst_gitCache)
#include "tst_gitcache.moc"

