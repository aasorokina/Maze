cmake_minimum_required(VERSION 3.5)

project(maze_tests VERSION 1.0 LANGUAGES CXX)

if(WIN32 AND DEFINED ENV{VCPKG_ROOT} AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
  set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake"
      CACHE STRING "")
endif()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
    if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
        message(STATUS "Enabling code coverage flags")
        add_compile_options(--coverage -O0 -g)
        add_link_options(--coverage)
    endif()
endif()

if(NOT DEFINED CMAKE_CXX_STANDARD) 
    set(CMAKE_CXX_STANDARD "20")
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
endif()

if(NOT DEFINED CMAKE_CXX_FLAGS) 
    set(CMAKE_CXX_FLAGS "-Wall -Werror -Wextra -g -std=c++${CMAKE_CXX_STANDARD}") 
endif()

# enable_testing()

find_package(GTest REQUIRED)

set(PROJECT_SOURCES
    test.h    
    ../include/maze.h 
    ../include/maze_builder.h
    test.cpp
    maze_generate_test.cpp
    maze_solve_test.cpp
    ../src/maze.cpp 
    ../src/maze_builder.cpp
)

add_executable(maze_tests ${PROJECT_SOURCES})

target_link_libraries(maze_tests PRIVATE GTest::gtest GTest::gtest_main)

