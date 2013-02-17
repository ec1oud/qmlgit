
CONFIG +=link_pkgconfig
PKGCONFIG += libgit2

SOURCES += \
    $$PWD/diffhighlighter.cpp \
    $$PWD/gitcommitlist.cpp \
    $$PWD/gitdiff.cpp \
    $$PWD/main.cpp \

HEADERS += \
    $$PWD/diffhighlighter.h \
    $$PWD/gitrepo.h \
    $$PWD/gitcommitlist.h \
    $$PWD/gitdiff.h \
