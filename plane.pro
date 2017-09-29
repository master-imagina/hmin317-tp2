QT       += core gui widgets

TARGET = plane
TEMPLATE = app

SOURCES += main.cpp \
    camera.cpp

SOURCES += \
    mainwidget.cpp \
    geometryengine.cpp

HEADERS += \
    mainwidget.h \
    geometryengine.h \
    camera.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

CONFIG += c++11

# install
target.path = .
INSTALLS += target
