
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

QTEST_MAIN(tst_gitCache)
#include "tst_gitcache.moc"

