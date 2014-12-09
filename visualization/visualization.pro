TEMPLATE = app

QT += qml quick widgets opengl openglextensions
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11

SOURCES += main.cpp \
    dsmc.cpp \
    simulator.cpp \
    dsmcrenderer.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    dsmc.h \
    simulator.h \
    dsmcrenderer.h
