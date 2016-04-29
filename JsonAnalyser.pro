#-------------------------------------------------
#
# Project created by QtCreator 2015-09-24T07:58:33
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = JsonAnalyser
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    newfromurldialog.cpp \
    myjsonmodel.cpp \
    myjsonmodelitem.cpp

HEADERS  += mainwindow.h \
    newfromurldialog.h \
    myjsonmodel.h \
    myjsonmodelitem.h

FORMS    += mainwindow.ui \
    newfromurldialog.ui

include(QtAwesome/QtAwesome.pri)
