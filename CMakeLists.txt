cmake_minimum_required(VERSION 3.5)

project(expunge VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        exewrapper.h exewrapper.cpp
)

if (APPLE)
    set(MACOSX_BUNDLE_ICON_FILE expunge.icns)
    set(app_icon_macos "${CMAKE_CURRENT_SOURCE_DIR}/expunge.icns")
    set_source_files_properties(${app_icon_macos} PROPERTIES
               MACOSX_PACKAGE_LOCATION "Resources")
    qt_add_executable(expunge
        ${PROJECT_SOURCES} ${app_icon_macos}
    )
elseif(WIN32)
    qt_add_executable(expunge
        ${PROJECT_SOURCES} expunge.rc
    )
else()
    qt_add_executable(expunge
        ${PROJECT_SOURCES}
    )
endif()

if (APPLE)
    file(DOWNLOAD https://wit.wiimm.de/download/wit-v3.05a-r8638-mac.tar.gz ${CMAKE_CURRENT_BINARY_DIR}/wit.tar.gz)
    add_custom_command(TARGET expunge POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar xzf ${CMAKE_CURRENT_BINARY_DIR}/wit.tar.gz
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/expunge.app/Contents/MacOS/wit
        COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/wit-v3.05a-r8638-mac ${CMAKE_CURRENT_BINARY_DIR}/expunge.app/Contents/MacOS/wit
    )
elseif(WIN32)
    file(DOWNLOAD https://wit.wiimm.de/download/wit-v3.05a-r8638-cygwin64.zip ${CMAKE_CURRENT_BINARY_DIR}/wit.zip)
    add_custom_command(TARGET expunge POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E tar xf ${CMAKE_CURRENT_BINARY_DIR}/wit.zip
        COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_CURRENT_BINARY_DIR}/wit
        COMMAND ${CMAKE_COMMAND} -E rename ${CMAKE_CURRENT_BINARY_DIR}/wit-v3.05a-r8638-cygwin64 ${CMAKE_CURRENT_BINARY_DIR}/wit
    )
endif()


target_link_libraries(expunge PRIVATE Qt6::Widgets)

set_target_properties(expunge PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER com.landmaster.expunge
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
