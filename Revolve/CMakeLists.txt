cmake_minimum_required(VERSION 3.16)
project(Revolve)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)  # Enable Qt MOC processing automatically

find_package(OpenGL REQUIRED)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

add_executable(Revolve
    main.cpp
    GLWidget.cpp
    GLWidget.h
)

target_link_libraries(Revolve
    PRIVATE Qt6::Core Qt6::Gui Qt6::Widgets Qt6::OpenGLWidgets OpenGL::GL
)
