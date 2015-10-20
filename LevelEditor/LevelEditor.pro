#-------------------------------------------------
#
# Project created by QtCreator 2015-09-17T12:58:15
#
#-------------------------------------------------

QT       += core gui
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LevelEditor
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newleveldialog.cpp \
    map.cpp \
    view.cpp \
    editorlevel.cpp \
    editorgame.cpp \
    texturehandler.cpp \
    editorunit.cpp \
    textureselectdialog.cpp \
    newghostmodedialog.cpp \
    ghosttimer.cpp

HEADERS  += mainwindow.h \
    newleveldialog.h \
    map.h \
    view.h \
    editorlevel.h \
    editorgame.h \
    texturehandler.h \
    editorunit.h \
    textureselectdialog.h \
    newghostmodedialog.h \
    ghosttimer.h

FORMS    += mainwindow.ui \
    newleveldialog.ui \
    textureselectdialog.ui \
    newghostmodedialog.ui

RESOURCES += \
    dependencies.qrc
