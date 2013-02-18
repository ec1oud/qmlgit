
# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

QT += quick widgets

include(src/src.pri)

RESOURCES += \
    resources.qrc

INCLUDEPATH += /home/frederik/dev/tools/libgit2/libgit2/include
INCLUDEPATH += /home/frederik/dev/tools/git/libgit2/libgit2/include

OTHER_FILES += qml/*
