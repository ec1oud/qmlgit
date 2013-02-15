
CONFIG +=link_pkgconfig
PKGCONFIG += libgit2

SOURCES += \
    $$PWD/gitcommitlist.cpp \
    $$PWD/gitdiff.cpp \
    $$PWD/main.cpp \

HEADERS += \
    $$PWD/gitcommitlist.h \
    $$PWD/gitdiff.h \
