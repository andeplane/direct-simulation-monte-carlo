include(../defaults.pri)

CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++11 -g

TEMPLATE = app

SOURCES += main.cpp

LIBS += -L../src -ldsmc
#LIBS += ../src/libdsmc.a
