cmake_minimum_required(VERSION 3.10)

project(3DGraphicsApp)

set(CMAKE_CXX_STANDARD 11)

include_directories(include)

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_executable(3DGraphicsApp ${SOURCES})

find_package(OpenGL REQUIRED)
find_package(GLM REQUIRED)

target_link_libraries(3DGraphicsApp ${OPENGL_LIBRARIES} glm)