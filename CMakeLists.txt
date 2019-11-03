cmake_minimum_required(VERSION 3.10)
project(ptk VERSION 1 DESCRIPTION "C++ library for building user interfaces.")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -pthread")

# Build Type
if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DPTK_DEBUG)
    set(PTK_BUILD_TYPE "Debug")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(PTK_BUILD_TYPE "Release")
else()
    message(STATUS "No build type specified. Default is Release")
    set(PTK_BUILD_TYPE "Release")
endif()
message(STATUS "pTK build: ${PTK_BUILD_TYPE}")

# OS
if (UNIX AND NOT APPLE)
    # for Linux, BSD, Solaris, Minix
    set(SKIA_PLATFORM_DEPENDENCIES "-lfreetype -lfontconfig")
else()
    set(SKIA_PLATFORM_DEPENDENCIES "")
endif()

# Compiler
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")    
    add_definitions(-DPTK_COMPILER_CLANG)
    message(STATUS "Compiler: Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions(-DPTK_COMPILER_GCC)
    message(STATUS "Compiler: GCC")
endif()

# Options
option(BUILD_TESTS "Build test programs" OFF)
option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_SANDBOX "Build sandbox application" ON)

# Print Options
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "Build Examples: ${BUILD_EXAMPLES}")
message(STATUS "Build Sandbox: ${BUILD_SANDBOX}")

# Source files
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/src/*.cpp)
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCE_FILES})

# Header files
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.hpp)
source_group(TREE "${CMAKE_SOURCE_DIR}/include/ptk" PREFIX "Header Files" FILES ${HEADER_FILES})

include_directories(${CMAKE_SOURCE_DIR}/include/)

# Skia properties
set(SKIA_PATH "${CMAKE_SOURCE_DIR}/third_party/skia")
set(SKIA_LIB_PATH "${SKIA_PATH}/out/${PTK_BUILD_TYPE}")
message(STATUS "Skia include path: ${SKIA_PATH}")
message(STATUS "Skia build: ${PTK_BUILD_TYPE}")
message(STATUS "Skia lib path: ${SKIA_LIB_PATH}")
message(STATUS "Skia platform dependencies: ${SKIA_PLATFORM_DEPENDENCIES}")

# Skia library
add_library(skia STATIC IMPORTED)
set_target_properties(skia PROPERTIES
    IMPORTED_LOCATION "${SKIA_LIB_PATH}/libskia.a"
    INTERFACE_INCLUDE_DIRECTORIES "${SKIA_PATH}"
)

# Define executable
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Skia
target_link_libraries(${PROJECT_NAME} ${SKIA_PLATFORM_DEPENDENCIES} skia)

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# GLFW
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL ON CACHE BOOL "" FORCE)
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/GLFW)
include_directories(${CMAKE_SOURCE_DIR}/third_party/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

# spdlog
set(SPDLOG_BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/spdlog)
spdlog_enable_warnings(${PROJECT_NAME})
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

# Sandbox
if (BUILD_SANDBOX)
    add_subdirectory(${CMAKE_SOURCE_DIR}/sandbox)
endif(BUILD_SANDBOX)
