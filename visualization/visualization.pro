include(../defaults.pri)
TEMPLATE = app
CONFIG -= app_bundle

QT += qml quick widgets opengl openglextensions
CONFIG += c++11
LIBS += -L../src -ldsmc

SOURCES += main.cpp \
    dsmc.cpp \
    simulator.cpp \
    dsmcrenderer.cpp \
    quads.cpp \
    points.cpp \
    scalarfield.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    dsmc.h \
    simulator.h \
    dsmcrenderer.h \
    quads.h \
    points.h \
    scalarfield.h
