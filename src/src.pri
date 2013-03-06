
CONFIG +=link_pkgconfig
PKGCONFIG += libgit2

SOURCES += \
    $$PWD/beziercurve.cpp \
    $$PWD/diffhighlighter.cpp \
    $$PWD/gitrepo.cpp \
    $$PWD/gitcommitlist.cpp \
    $$PWD/main.cpp \

HEADERS += \
    $$PWD/beziercurve.h \
    $$PWD/diffhighlighter.h \
    $$PWD/gitrepo.h \
    $$PWD/gitcommitlist.h \
