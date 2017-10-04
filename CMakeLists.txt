cmake_minimum_required(VERSION 3.5)

project(gameenginepw)

# C++ language version
set(CMAKE_CXX_STANDARD 14)

# Files
set(HEADERS
    aabb.h
    cameraactions.h
    coordconversions.h
    gamewidget.h
    geometry.h
    heightmap.h
    renderer.h
    camera.h
    cameracontroller.h
    gameloop.h
    mainwindow.h
    )

set(SOURCES
    aabb.cpp
    cameraactions.cpp
    coordconversions.cpp
    gamewidget.cpp
    geometry.cpp
    heightmap.cpp
    renderer.cpp
    camera.cpp
    cameracontroller.cpp
    mainwindow.cpp
    gameloop.cpp
    main.cpp
    )

set(QT_RESOURCES
    shaders.qrc)

# Packages
#  Qt5
find_package(Qt5 COMPONENTS Widgets REQUIRED)
set(CMAKE_AUTOMOC true)
set(CMAKE_AUTORCC true)

#  Binary
add_executable(pw2 ${SOURCES} ${HEADERS} ${QT_RESOURCES})

target_link_libraries(pw2 Qt5::Widgets)
