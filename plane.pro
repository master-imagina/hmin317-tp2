QT       += core gui widgets

TARGET = plane
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h

RESOURCES += \
    shaders.qrc \
    textures.qrc \
    images.qrc

# install
target.path = .
INSTALLS += target

DISTFILES += \
    heightmap-11.png
