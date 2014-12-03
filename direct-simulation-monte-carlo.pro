TEMPLATE = subdirs
CONFIG+=ordered
CONFIG += c++11
SUBDIRS += \
    src \
    app

app.depends = src

OTHER_FILES += \
    defaults.pri
