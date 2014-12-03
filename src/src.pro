include(../defaults.pri)

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt
CONFIG   += c++11
QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = lib

TARGET = dsmc

SOURCES += system.cpp \
    cell.cpp \
    particles.cpp \
    vec3.cpp \
    random.cpp \
    unitconverter.cpp \
    settings.cpp \
    filemanager.cpp \
    particlemover.cpp \
    cpelapsedtimer.cpp
HEADERS += system.h \
    cell.h \
    particles.h \
    config.h \
    vec3.h \
    random.h \
    unitconverter.h \
    settings.h \
    filemanager.h \
    particlemover.h \
    cpelapsedtimer.h
