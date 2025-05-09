cmake_minimum_required(VERSION 3.19)

project(qt-app LANGUAGES CXX)

find_package(Qt6 6.9.0 REQUIRED COMPONENTS Core Gui Widgets OpenGL OpenGLWidgets)

qt_standard_project_setup()

include_directories(${CMAKE_SOURCE_DIR}/include)

qt_add_executable(qt-app

    WIN32 MACOSX_BUNDLE

    main.cpp

    RobotWidget.h

    robotwidget.cpp

)

target_link_libraries(qt-app

    PRIVATE

        Qt6::Core

        Qt6::Gui

        Qt6::Widgets

        Qt6::OpenGL

        Qt6::OpenGLWidgets

        opengl32

)

include(GNUInstallDirs)

install(TARGETS qt-app

    BUNDLE  DESTINATION .

    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}

    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}

)

qt_generate_deploy_app_script(

    TARGET qt-app

    OUTPUT_SCRIPT deploy_script

    NO_UNSUPPORTED_PLATFORM_ERROR

)

install(SCRIPT ${deploy_script})
 