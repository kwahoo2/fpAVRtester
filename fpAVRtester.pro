#-------------------------------------------------
#
# Project created by QtCreator 2015-10-14T19:25:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets serialport

TARGET = fpAVRtester
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp \
    serialreader.cpp

HEADERS  += mainwindow.h \
    serialreader.h

FORMS    += mainwindow.ui
