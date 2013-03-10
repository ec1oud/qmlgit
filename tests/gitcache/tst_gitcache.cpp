
#include <QtTest/QtTest>

class tst_gitCache : public QObject
{
    Q_OBJECT
private slots:
    void tags();
};

void tst_gitCache::tags()
{
}

QTEST_MAIN(tst_gitCache)
#include "tst_gitcache.moc"

