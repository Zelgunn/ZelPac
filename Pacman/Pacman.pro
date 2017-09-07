#-------------------------------------------------
#
# Project created by QtCreator 2015-09-18T10:50:18
#
#-------------------------------------------------

QT       += core gui
QT       += xml
QT       += serialport
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Pacman
TEMPLATE = app


SOURCES += main.cpp\
    grid.cpp \
    unit.cpp \
    ghost.cpp \
    game.cpp \
    level.cpp \
    pacscreen.cpp \
    arduinohandler.cpp \
    doubleposition.cpp \
    pacmanunit.cpp \
    ghosttimer.cpp \
    texturehandler.cpp \
    firework.cpp

HEADERS  += \
    grid.h \
    unit.h \
    ghost.h \
    game.h \
    level.h \
    pacmanparameters.h \
    pacscreen.h \
    arduinohandler.h \
    doubleposition.h \
    pacmanunit.h \
    ghosttimer.h \
    texturehandler.h \
    firework.h

FORMS    +=

RESOURCES += \
    depedencies.qrc

win32:CONFIG(debug, debug|release): LIBS += -LD:/SFML/lib/ -lsfml-system-d -lsfml-graphics-d -lsfml-window-d
else:win32:CONFIG(release, debug|release): LIBS += -LD:/SFML/lib/ -lsfml-system -lsfml-graphics -lsfml-window

#MSVC_2012_64

INCLUDEPATH += D:/SFML/include
DEPENDPATH += D:/SFML/include
