#-------------------------------------------------
#
# Project created by QtCreator 2015-08-14T18:56:19
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Megatron-server
TEMPLATE = app


SOURCES += main.cpp\
    server.cpp \
    qcan.cpp \
    CANOpenShellSlaveOD.c \
    cancontrol.cpp

HEADERS  += server.h \
    ../common.h \
    qcan.h \
    CANOpenShellSlaveOD.h \
    cancontrol.h

INCLUDEPATH += /usr/local/include/canfestival
LIBS += -L/usr/local/lib -lcanfestival -lcanfestival_unix -ldl -lrt

FORMS    += server.ui
