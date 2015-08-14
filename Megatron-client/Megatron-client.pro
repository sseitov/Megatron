#-------------------------------------------------
#
# Project created by QtCreator 2015-08-14T20:20:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Megatron-client
TEMPLATE = app


SOURCES += main.cpp\
        client.cpp \
    buttonsetup.cpp \
    inputbutton.cpp

HEADERS  += client.h \
    buttonsetup.h \
    inputbutton.h

FORMS    += client.ui \
    buttonsetup.ui
