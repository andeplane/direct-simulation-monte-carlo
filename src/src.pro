include(../defaults.pri)

CONFIG   += console
CONFIG   -= app_bundle
CONFIG   -= qt
CONFIG   += c++11
QMAKE_CXXFLAGS += -std=c++11 -xCORE-AVX-I -O3 -ipo -g

#DEFINES += DSMC_DEBUG

#CONFIG += staticlib
#TEMPLATE = lib
TEMPLATE = app

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
    cpelapsedtimer.cpp \
    cellmanager.cpp \
    statisticssampler.cpp \
    grid.cpp \
    vec2.cpp \
    main.cpp
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
    cpelapsedtimer.h \
    cellmanager.h \
    statisticssampler.h \
    grid.h \
    vec2.h
