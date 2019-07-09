cmake_minimum_required(VERSION 3.10)
project(ptk)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

# Build Type
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DPTK_DEBUG)
    set(PTK_BUILD_TYPE "Debug")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(PTK_BUILD_TYPE "Release")
else()
    set(PTK_BUILD_TYPE "Release")
endif()

# Options
option(BUILD_TESTS "Build test programs" OFF)
option(BUILD_EXAMPLES "Build example programs" ON)

# Source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/include/*.hpp)

include_directories(${CMAKE_SOURCE_DIR}/include/)

# Skia Path and include folder
set(SKIA_SDK "${CMAKE_SOURCE_DIR}/third_party/skia")
include_directories(${SKIA_SDK})

# Define executable
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Skia
if (EXISTS ${SKIA_SDK}/out/${PTK_BUILD_TYPE}/libskia.a)
    message(STATUS "Skia build type: ${PTK_BUILD_TYPE}")
    target_link_libraries(${PROJECT_NAME} ${SKIA_SDK}/out/${PTK_BUILD_TYPE}/libskia.a)
else()
    message(FATAL_ERROR "pTK does not yet support dynamic linking of skia")
endif()

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# GLFW
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/glfw)
include_directories(${CMAKE_SOURCE_DIR}/third_party/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

# spdlog
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/spdlog)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

# Example
if (BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/example)
endif (BUILD_EXAMPLES)

# Tests
if (BUILD_TESTS)
enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif (BUILD_TESTS)
