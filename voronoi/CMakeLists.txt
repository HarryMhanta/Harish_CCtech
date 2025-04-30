cmake_minimum_required(VERSION 3.16)
project(voronoi)

set(CMAKE_CXX_STANDARD 17)

# Enable moc/uic/rcc processing
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# Find required Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

# Include your source directory
include_directories(${PROJECT_SOURCE_DIR}/include)

# Gather source files
file(GLOB SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
    ${PROJECT_SOURCE_DIR}/include/*.h
)

# Create executable
add_executable(voronoi ${SOURCES})

# Link to Qt and OpenGL
target_link_libraries(voronoi PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGL
    Qt6::OpenGLWidgets
    #Qt6::openGLFunctions
    opengl32
)
