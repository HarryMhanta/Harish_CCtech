cmake_minimum_required(VERSION 3.10)
project(ProjectDraw)
 
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
 
 
# Enable Qt support
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/application/ui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)    
 
# Include directories
set(INCLUDE_DIRECTORIES
    ${CMAKE_SOURCE_DIR}/Geometry/include
    ${CMAKE_SOURCE_DIR}/application/include
    ${CMAKE_SOURCE_DIR}/external
)
 
include_directories(${INCLUDE_DIRECTORIES})
 
# Source files
file(GLOB GEOMETRY_SRC ${CMAKE_SOURCE_DIR}/Geometry/src/*.cpp)
file(GLOB APPLICATION_SRC
    ${CMAKE_SOURCE_DIR}/application/src/MainWindow.cpp
    ${CMAKE_SOURCE_DIR}/application/include/MainWindow.h
    ${CMAKE_SOURCE_DIR}/application/src/*.cpp
    # ${CMAKE_SOURCE_DIR}/application/ui/*.ui
)
 
file(GLOB OPENGL_SRC
    ${CMAKE_SOURCE_DIR}/application/src/GLWidget.cpp
    ${CMAKE_SOURCE_DIR}/application/include/GLWidget.h
    ${CMAKE_SOURCE_DIR}/application/src/PolygonExtrusionwidget.cpp
    ${CMAKE_SOURCE_DIR}/application/include/PolygonExtrusionwidget.h
)
list(APPEND APPLICATION_SRC ${OPENGL_SRC})
 
 
# Find Qt
find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(OpenGL REQUIRED)
 
add_library(Geometry SHARED ${GEOMETRY_SRC})
target_link_libraries(Geometry Qt6::Widgets)
 
add_executable(main ${APPLICATION_SRC} "application/src/MainWindow.cpp")
target_link_libraries(main Geometry Qt6::Widgets Qt6::OpenGLWidgets OpenGL::GL)