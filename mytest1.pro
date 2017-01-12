#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T10:18:18
#
#-------------------------------------------------

QT       += core gui printsupport opengl

#LIBS += opengl32.lib
QMAKE_CXXFLAGS += -std=c++0x

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mytest1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    form.cpp \
    linearalgebrasolver.cpp \
    feaanalysispanel.cpp \
    feaelementbase.cpp \
    stringelement.cpp \
    barelement.cpp \
    eulerbernoullibeam.cpp \
    materialele.cpp \
    geometryele.cpp \
    mathextension.cpp \
    mesh.cpp \
    openfoamqtextstream.cpp \
    coordsystem.cpp \
    stlmesh.cpp \
    viewerwindow.cpp \
    sph3dprintpanel.cpp

HEADERS  += mainwindow.h \
    form.h \
    linearalgebrasolver.h \
    feaanalysispanel.h \
    feaelementbase.h \
    stringelement.h \
    barelement.h \
    elementstyle.h \
    eulerbernoullibeam.h \
    materialele.h \
    geometryele.h \
    mathextension.h \
    mesh.h \
    openfoamqtextstream.h \
    coordsystem.h \
    stlmesh.h \
    viewerwindow.h \
    sph3dprintpanel.h \
    bound.h

FORMS    += mainwindow.ui \
    form.ui \
    feaanalysispanel.ui \
    sph3dprintpanel.ui

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    UI.png \
    lib.note
