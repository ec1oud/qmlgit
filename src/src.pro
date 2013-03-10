TARGET = qmlgit
CONFIG += link_pkgconfig
CONFIG += console

QT += quick widgets testlib
PKGCONFIG += libgit2

RESOURCES += \
    ../resources.qrc

OTHER_FILES += ../qml/*

SOURCES += \
    $$PWD/beziercurve.cpp \
    $$PWD/diffhighlighter.cpp \
    $$PWD/gitcache.cpp \
    $$PWD/gitrepo.cpp \
    $$PWD/gitcommitlist.cpp \
    $$PWD/main.cpp \

HEADERS += \
    $$PWD/beziercurve.h \
    $$PWD/diffhighlighter.h \
    $$PWD/gitcache.h \
    $$PWD/gitrepo.h \
    $$PWD/gitcommitlist.h \
