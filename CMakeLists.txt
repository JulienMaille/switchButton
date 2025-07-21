cmake_minimum_required(VERSION 3.19)

project(SwitchButton LANGUAGES CXX)

# Option to build the plugin
option(BUILD_DESIGNER_PLUGIN "Build the Designer plugin" OFF)

# Conditionally build the plugin or the interface
if(BUILD_DESIGNER_PLUGIN)
    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF) # Use strict C++17

    # Enable Qt automatic features
    set(CMAKE_AUTOMOC ON)
    set(CMAKE_AUTORCC ON)

    # Qt setup
    find_package(QT NAMES Qt5 Qt6 REQUIRED COMPONENTS Designer)
    find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Designer)

    # Plugin source files
    set(SOURCES
        SwitchButton.cpp
        SwitchButton.h
        SwitchButtonPlugin.cpp
        SwitchButtonPlugin.h
    # resources
        designericons.qrc
    )

    # Create the plugin library
    add_library(${PROJECT_NAME} SHARED ${SOURCES})

    # Build type suffix for library name
    set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_debug")

    # Force all config types to output into the top-level bin folder
    set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}/bin"
        RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}/bin"
    )

    # Link against Qt Designer
    target_link_libraries(${PROJECT_NAME} PRIVATE
        Qt::Designer
    )

    # Install plugin to Qt Designer plugin path
    install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${_qt5Designer_install_prefix}/plugins/designer
    )
else()
    # Create an interface target for the SwitchButton library
    add_library( ${PROJECT_NAME} INTERFACE)
    target_include_directories(${PROJECT_NAME} INTERFACE .)
    target_sources(${PROJECT_NAME} INTERFACE
        SwitchButton.cpp
        SwitchButton.h
    )

    message(STATUS "Configuring ${PROJECT_NAME} interface")
endif()