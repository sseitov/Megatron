#-------------------------------------------------
#
# Project created by QtCreator 2015-08-14T18:56:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += network widgets

TARGET = Megatron-server
TEMPLATE = app


SOURCES += main.cpp\
        server.cpp

HEADERS  += server.h \
    ../common.h

FORMS    += server.ui
