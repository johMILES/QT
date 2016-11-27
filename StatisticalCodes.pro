#-------------------------------------------------
#
# Project created by QtCreator 2016-11-23T10:27:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = StatisticalCodes
TEMPLATE = app


SOURCES += main.cpp\
        codewidget.cpp \
    statisticalfunction.cpp \
    statisticalthread.cpp \
    parameter.cpp

HEADERS  += codewidget.h \
    statisticalfunction.h \
    statisticalthread.h \
    parameter.h

FORMS    += codewidget.ui \
    parameter.ui
