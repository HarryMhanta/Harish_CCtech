cmake_minimum_required(VERSION 3.14)
project(CUBEMAP)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)

# Find required Qt modules
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

# Add sources
set(SOURCES
    glwidget.h
    mainwindow.h
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link required Qt libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
)

target_sources(CUBEMAP PRIVATE
    glwidget.cpp
    main.cpp
    mainwindow.cpp
)
