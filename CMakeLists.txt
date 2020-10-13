cmake_minimum_required(VERSION 3.15.7)
cmake_policy(SET CMP0091 NEW)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(ptk VERSION 1 DESCRIPTION "C++ library for building user interfaces.")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

# General options
option(BUILD_TESTS "Build test programs" OFF)
option(BUILD_EXAMPLES "Build example programs" ON)
option(BUILD_SANDBOX "Build sandbox application" ON)

# Hardware acceleration options.
option(ENABLE_OPENGL "Enable OpenGL backend" ON)
option(ENABLE_METAL "Enable Metal backend" ON)

# Compiler
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_definitions(-DPTK_COMPILER_CLANG)
    message(STATUS "Compiler: Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_definitions(-DPTK_COMPILER_GCC)
    message(STATUS "Compiler: GCC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
	add_definitions(-DPTK_COMPILER_MSVC)
	message(STATUS "Compiler: MSVC")
endif()

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
    add_definitions(-DPTK_PLATFORM_UNIX)
    set(PTK_DEPENDENCIES "-ldl -lfreetype -lfontconfig")
	set(SKIA_FILENAME "libskia.a")
    set(PTK_PLATFORM "Unix")
elseif (WIN32)
    add_definitions(-DPTK_PLATFORM_WINDOWS)
    set(PTK_PLATFORM "Windows")
	set(SKIA_FILENAME "skia.lib")
    if (ENABLE_OPENGL)
        set(PTK_DEPENDENCIES "opengl32")
        add_definitions(-DSK_GL)
    endif (ENABLE_OPENGL)
elseif(APPLE)
    add_definitions(-DPTK_PLATFORM_APPLE)
    set(PTK_PLATFORM "macOS")
    set(PTK_DEPENDENCIES "-framework CoreFoundation -framework CoreGraphics -framework CoreText -framework CoreServices -framework AppKit -framework QuartzCore")
    if (ENABLE_METAL)
        set(PTK_DEPENDENCIES ${PTK_DEPENDENCIES} "-framework Metal")
        add_definitions(-DSK_METAL)
    endif (ENABLE_METAL)
	set(SKIA_FILENAME "libskia.a")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()
message(STATUS "pTK platform: ${PTK_PLATFORM}")
message(STATUS "pTK dependencies: ${PTK_DEPENDENCIES}")

# Define HW if enabled.
if (ENABLE_METAL AND APPLE)
    add_definitions(-DPTK_METAL)
endif (ENABLE_METAL AND APPLE)
if (ENABLE_OPENGL AND WIN32)
    add_definitions(-DPTK_OPENGL)
endif (ENABLE_OPENGL AND WIN32)

# Skia properties
set(SKIA_PATH "${CMAKE_SOURCE_DIR}/third_party/skia")
set(SKIA_LIB "${SKIA_PATH}/out/${PTK_BUILD_TYPE}/${SKIA_FILENAME}")
message(STATUS "Skia build: ${PTK_BUILD_TYPE}")
if (SKIA_PLATFORM_DEPENDENCIES)
	message(STATUS "Skia platform dependencies: ${SKIA_PLATFORM_DEPENDENCIES}")
endif()
message(STATUS "Skia include path: ${SKIA_PATH}")
message(STATUS "Skia lib: ${SKIA_LIB}")

# Skia library
add_library(skia STATIC IMPORTED)
set_target_properties(skia PROPERTIES
    IMPORTED_LOCATION "${SKIA_LIB}"
    INTERFACE_INCLUDE_DIRECTORIES "${SKIA_PATH}"
)

# HW Acceleration Packages
if (ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)
endif (ENABLE_OPENGL)

# Threads
find_package (Threads REQUIRED)

# Define library
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

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

# Print options
message(STATUS "Build Tests: ${BUILD_TESTS}")
message(STATUS "Build Examples: ${BUILD_EXAMPLES}")
message(STATUS "Build Sandbox: ${BUILD_SANDBOX}")
