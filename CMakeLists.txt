cmake_minimum_required(VERSION 3.6)
project(TP2_Moteur)

set(CMAKE_CXX_STANDARD 11)

find_package(OpenGL REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(OpenMP REQUIRED)
include_directories( ${OPENGL_INCLUDE_DIRS}  ${GLUT_INCLUDE_DIRS} )


set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)

set(SOURCE_FILES
        main.cpp mainwidget.cpp geometryengine.cpp)

set(HEADER_FILES
        mainwidget.h geometryengine.h)

set(QT_RES
        shaders.qrc textures.qrc)

set(SHADER_FILES
        fshader.glsl vshader.glsl)

add_executable(TP1 ${SOURCE_FILES} ${HEADER_FILES} ${QT_RES} ${SHADER_FILES})
target_link_libraries(TP1 ${OPENGL_LIBRARIES} Qt5::Widgets ${OpenMP_CXX_FLAGS})