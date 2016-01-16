#-------------------------------------------------
#
# Project created by QtCreator 2015-11-24T10:38:44
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Megatron-video
TEMPLATE = app

MOC_DIR = tmp
UI_DIR = tmp
RCC_DIR = tmp

SOURCES += main.cpp\
        videoclient.cpp \
        buttonsetup.cpp \
        controlbutton.cpp

HEADERS  += videoclient.h \
        ../common.h \
        buttonsetup.h \
        controlbutton.h

FORMS    += videoclient.ui \
	buttonsetup.ui
