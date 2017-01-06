#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T10:18:18
#
#-------------------------------------------------

QT       += core gui printsupport opengl

LIBS += opengl32.lib

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mytest1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    form.cpp \
    linearalgebrasolver.cpp \
    feaanalysispanel.cpp \
    feaviewer.cpp

HEADERS  += mainwindow.h \
    form.h \
    linearalgebrasolver.h \
    feaanalysispanel.h \
    feaviewer.h

FORMS    += mainwindow.ui \
    form.ui \
    feaanalysispanel.ui

RESOURCES += \
    resource.qrc
