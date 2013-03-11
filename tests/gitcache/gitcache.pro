TARGET = tst_gitcache
CONFIG += testcase
QT = core testlib

#INCLUDEPATH += $$PWD/../../src

CONFIG += link_pkgconfig
PKGCONFIG += libgit2

SOURCES += \
    $$PWD/tst_gitcache.cpp \
#    $$PWD/../../src/gitcache.cpp
