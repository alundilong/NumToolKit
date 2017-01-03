#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T10:18:18
#
#-------------------------------------------------

QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mytest1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    form.cpp \
    linearalgebrasolver.cpp

HEADERS  += mainwindow.h \
    form.h \
    linearalgebrasolver.h

FORMS    += mainwindow.ui \
    form.ui

RESOURCES += \
    resource.qrc
