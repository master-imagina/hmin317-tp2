cmake_minimum_required(VERSION 3.5)
project(tp2)

set(CMAKE_CXX_STANDARD 14)

# Sources
set(SRC_FILES main.cpp mainwidget.cpp mainwidget.h geometryengine.cpp geometryengine.h)

# Add Qt5
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Gui REQUIRED)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
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

# Create executable
add_executable(tp2 ${SOURCE_FILES} ${HEADER_FILES} ${QT_RES} ${SHADER_FILES})
target_link_libraries(tp2 ${OPENGL_LIBRARIES} Qt5::Widgets ${OpenMP_CXX_FLAGS})
