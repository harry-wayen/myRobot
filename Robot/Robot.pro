#-------------------------------------------------
#
# Project created by QtCreator 2017-04-12T17:27:41
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Robot
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp

HEADERS  += widget.h

FORMS    += widget.ui
include(qextserialport/src/qextserialport.pri)
