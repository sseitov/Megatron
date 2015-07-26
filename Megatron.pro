#-------------------------------------------------
#
# Project created by QtCreator 2015-07-05T08:02:22
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Megatron
TEMPLATE = app


SOURCES += src/main.cpp\
    src/controllerwidget.cpp \
    src/qjoystick.cpp

HEADERS  += src/controllerwidget.h \
    src/qjoystick.h

macx {
INCLUDEPATH += /Library/Frameworks/SDL.framework/Headers
OBJECTIVE_SOURCES += src/SDLMain.m
HEADERS += src/SDLMain.h
LIBS += -F/Library/Frameworks
LIBS += -framework Cocoa -framework SDL
}

linux {
SOURCES += src/qcan.cpp src/ObjDict.c
HEADERS  += src/qcan.h src/ObjDict.h

INCLUDEPATH += /usr/local/include/canfestival
LIBS += -lSDL
LIBS += -L/usr/local/lib -lcanfestival -lcanfestival_unix -ldl -lrt
}

FORMS    += src/controllerwidget.ui
