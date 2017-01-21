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

SOURCES += \
    src/container/List.cpp \
    src/coordinateSystem/CoordSystem.cpp \
    src/fea/element/BarElement.cpp \
    src/fea/element/EulerBernoulliBeam.cpp \
    src/fea/element/FeaElementBarThreeD.cpp \
    src/fea/element/FeaElementBase.cpp \
    src/fea/element/FeaElementLinearCubicalElement.cpp \
    src/fea/element/FeaElementOneD.cpp \
    src/fea/element/FeaElementThreeD.cpp \
    src/fea/element/FeaElementTwoD.cpp \
    src/fea/element/StringElement.cpp \
    src/fea/geometry/GeometryEle.cpp \
    src/fea/material/MaterialEle.cpp \
    src/fea/math/LinearAlgebraSolver.cpp \
    src/fea/math/Mathextension.cpp \
    src/fea/mesh/FeaMesh.cpp \
    src/fea/mesh/Mesh.cpp \
    src/fea/mesh/OpenFoamQTextstream.cpp \
    src/fea/mesh/STLMesh.cpp \
    src/ui/FeaAnlysisPanel.cpp \
    src/ui/Form.cpp \
    src/ui/main.cpp \
    src/ui/MainWindow.cpp \
    src/ui/SPH3DPrintPanel.cpp \
    src/ui/ViewerWindow.cpp

HEADERS  += \
    src/container/List.h \
    src/coordinateSystem/CoordSystem.h \
    src/fea/element/BarElement.h \
    src/fea/element/ElementStyle.h \
    src/fea/element/EulerBernoulliBeam.h \
    src/fea/element/FeaElementBarThreeD.h \
    src/fea/element/FeaElementBase.h \
    src/fea/element/FeaElementLinearCubicalElement.h \
    src/fea/element/FeaElementOneD.h \
    src/fea/element/FeaElementThreeD.h \
    src/fea/element/FeaElementTwoD.h \
    src/fea/element/StringElement.h \
    src/fea/geometry/GeometryEle.h \
    src/fea/material/MaterialEle.h \
    src/fea/material/MaterialOne.h \
    src/fea/math/LinearAlgebraSolver.h \
    src/fea/math/MathExtension.h \
    src/fea/math/MathExtensionIO.h \
    src/fea/math/MathExtensionTemplate.h \
    src/fea/mesh/Bound.h \
    src/fea/mesh/FeaMesh.h \
    src/fea/mesh/Mesh.h \
    src/fea/mesh/OpenFoamQTextstream.h \
    src/fea/mesh/STLMesh.h \
    src/macro/RunTimeSelectionTables.h \
    src/ui/FeaAnalysisPanel.h \
    src/ui/Form.h \
    src/ui/MainWindow.h \
    src/ui/RunTimeSelectionTables.h \
    src/ui/SPH3DPrintPanel.h \
    src/ui/ViewerWindow.h

FORMS    += \
    src/ui/feaanalysispanel.ui \
    src/ui/form.ui \
    src/ui/mainwindow.ui \
    src/ui/sph3dprintpanel.ui

RESOURCES += \
    resource.qrc

OTHER_FILES += \
    UI.png \
    lib.note
