cmake_minimum_required(VERSION 3.10)
project(ptk)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

# Source files
file(GLOB_RECURSE SOURCE_FILES
        ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Header files
file(GLOB_RECURSE HEADER_FILES
        ${CMAKE_SOURCE_DIR}/include/*.hpp)

include_directories(${CMAKE_SOURCE_DIR}/include/)

# Skia Path and include folder
set(SKIA_SDK "${CMAKE_SOURCE_DIR}/vendor/skia")
include_directories(${SKIA_SDK})

# Define executable
add_library(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES})

# Skia
if (CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(${PROJECT_NAME} ${SKIA_SDK}/out/Debug/libskia.a)
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    target_link_libraries(${PROJECT_NAME} ${SKIA_SDK}/out/Release/libskia.a)
endif()

# OpenGL
find_package(OpenGL REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# GLFW
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/glfw)
include_directories(${CMAKE_SOURCE_DIR}/vendor/glfw/include)
target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})

# spdlog
add_subdirectory(${CMAKE_SOURCE_DIR}/vendor/spdlog)
target_link_libraries(${PROJECT_NAME} spdlog::spdlog)

# Example
add_subdirectory(${CMAKE_SOURCE_DIR}/example)
