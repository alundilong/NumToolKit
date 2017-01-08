#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T10:18:18
#
#-------------------------------------------------

QT       += core gui printsupport opengl

LIBS += opengl32.lib
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mytest1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    form.cpp \
    linearalgebrasolver.cpp \
    feaanalysispanel.cpp \
    feaviewer.cpp \
    feaelementbase.cpp \
    stringelement.cpp \
    barelement.cpp

HEADERS  += mainwindow.h \
    form.h \
    linearalgebrasolver.h \
    feaanalysispanel.h \
    feaviewer.h \
    feaelementbase.h \
    stringelement.h \
    barelement.h \
    elementstyle.h

FORMS    += mainwindow.ui \
    form.ui \
    feaanalysispanel.ui

RESOURCES += \
    resource.qrc
