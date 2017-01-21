#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T10:18:18
#
#-------------------------------------------------

QT       += core gui printsupport opengl

#LIBS += opengl32.lib
QMAKE_CXXFLAGS += -std=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mytest1
TEMPLATE = app

SOURCES += main.cpp\
    BarElement.cpp \
    CoordSystem.cpp \
    EulerBernoulliBeam.cpp \
    FeaElementBarThreeD.cpp \
    FeaElementBase.cpp \
    FeaElementLinearCubicalElement.cpp \
    FeaElementOneD.cpp \
    FeaElementThreeD.cpp \
    FeaElementTwoD.cpp \
    Form.cpp \
    GeometryEle.cpp \
    LinearAlgebraSolver.cpp \
    MainWindow.cpp \
    MaterialEle.cpp \
    Mathextension.cpp \
    Mesh.cpp \
    OpenFoamQTextstream.cpp \
    SPH3DPrintPanel.cpp \
    STLMesh.cpp \
    StringElement.cpp \
    ViewerWindow.cpp \
    FeaAnlysisPanel.cpp \
    List.cpp

HEADERS  += \
    MaterialOne.h \
    BarElement.h \
    Bound.h \
    CoordSystem.h \
    ElementStyle.h \
    EulerBernoulliBeam.h \
    FeaAnalysisPanel.h \
    FeaElementBarThreeD.h \
    FeaElementBase.h \
    FeaElementLinearCubicalElement.h \
    FeaElementOneD.h \
    FeaElementThreeD.h \
    FeaElementTwoD.h \
    Form.h \
    GeometryEle.h \
    LinearAlgebraSolver.h \
    MainWindow.h \
    MaterialEle.h \
    MathExtension.h \
    MathExtensionIO.h \
    MathExtensionTemplate.h \
    Mesh.h \
    OpenFoamQTextstream.h \
    RunTimeSelectionTables.h \
    STLMesh.h \
    StringElement.h \
    ViewerWindow.h \
    SPH3DPrintPanel.h \
    License.h \
    List.h

FORMS    += mainwindow.ui \
    form.ui \
    feaanalysispanel.ui \
    sph3dprintpanel.ui

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    UI.png \
    lib.note
