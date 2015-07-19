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
    src/qjoystick.cpp \
    src/qcan.cpp \
    src/2088C.c

HEADERS  += src/controllerwidget.h \
    src/qjoystick.h \
    src/qcan.h \
    src/2088C.h

macx {
INCLUDEPATH += /Library/Frameworks/SDL.framework/Headers
OBJECTIVE_SOURCES += src/SDLMain.m
HEADERS += src/SDLMain.h
LIBS += -F/Library/Frameworks
LIBS += -framework Cocoa -framework SDL
}

linux {
INCLUDEPATH += /usr/local/include/canfestival
LIBS += -lSDL
LIBS += -L/usr/local/lib -lcanfestival_unix -lcanfestival -ldl -lrt
}

FORMS    += src/controllerwidget.ui
