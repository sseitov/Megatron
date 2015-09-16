#-------------------------------------------------
#
# Project created by QtCreator 2015-08-14T20:20:01
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Megatron-client
TEMPLATE = app

MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

SOURCES += main.cpp\
    client.cpp \
    qjoystick.cpp \
    buttonsetup.cpp \
    joystickmonitor.cpp \
    controlbutton.cpp

HEADERS  += client.h \
    qjoystick.h \
    buttonsetup.h \
    ../common.h \
    joystickmonitor.h \
    controlbutton.h

macx {
    OBJECTIVE_HEADERS += SDLMain.h
    OBJECTIVE_SOURCES += SDLMain.m
    INCLUDEPATH += /Library/Frameworks/SDL.framework/Headers
    LIBS += -framework Cocoa
    LIBS += -F/Library/Frameworks
    LIBS += -framework SDL
}

linux {
    LIBS += -lSDL
}

FORMS    += client.ui \
    buttonsetup.ui

RESOURCES += \
    images.qrc

OTHER_FILES += \
    images/target.png
