cmake_minimum_required(VERSION 3.5)

if(WIN32 AND DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

if(WIN32 AND DEFINED ENV{VCPKG_ROOT})
    set(CMAKE_PREFIX_PATH "C:/vcpkg/installed/x64-windows/share/cmake/Qt5")
endif()

project(Maze VERSION 1.0 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if(NOT DEFINED CMAKE_CXX_STANDARD) 
    set(CMAKE_CXX_STANDARD "20")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(NOT DEFINED CMAKE_CXX_FLAGS) 
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -g -std=c++${CMAKE_CXX_STANDARD}") 
endif()

find_package(QT NAMES Qt6 Qt5 REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include ${CMAKE_CURRENT_SOURCE_DIR}/src ${CMAKE_CURRENT_SOURCE_DIR}/resources)

file(GLOB_RECURSE SRC_CPP CONFIGURE_DEPENDS src/*.cpp)
file(GLOB_RECURSE SRC_H   CONFIGURE_DEPENDS include/*.h)
file(GLOB_RECURSE UI_FILES CONFIGURE_DEPENDS resources/*.ui)
file(GLOB_RECURSE QRC_FILES CONFIGURE_DEPENDS resources/*.qrc)

set(PROJECT_SOURCES
    ${SRC_CPP}
    ${SRC_H}
    ${UI_FILES}
    ${QRC_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Maze
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    else()
        add_executable(Maze
            ${PROJECT_SOURCES}
        )
endif()

target_link_libraries(Maze PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

set_target_properties(Maze PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Maze)
endif()
