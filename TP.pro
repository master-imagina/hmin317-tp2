QT       += core gui widgets

TARGET = TP
TEMPLATE = app

SOURCES += main.cpp \
    graphicmanager.cpp \ 
    camera.cpp \
    camerafreefly.cpp \
    cameraview.cpp \
    drawing.cpp \
    global.cpp \
    inputmanager.cpp \
    terrain.cpp

SOURCES +=

HEADERS += \
    graphicmanager.h \ 
    camera.h \
    camerafreefly.h \
    cameraview.h \
    drawing.h \
    global.h \
    inputmanager.h \
    terrain.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
target.path = $$[QT_INSTALL_EXAMPLES]/opengl/cube
INSTALLS += target

DISTFILES +=
