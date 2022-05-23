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

# Available HW on platforms.
set(PTK_OPENGL_PLATFORMS "Windows" "Unix")
set(PTK_METAL_PLATFORMS "Apple")

# Specific Debug Defines
option(PTK_CB_STORAGE_DEBUG "Enable Callback Storage debug printing" OFF)
IF(PTK_CB_STORAGE_DEBUG)
    ADD_DEFINITIONS(-DPTK_CB_STORAGE_DEBUG)
ENDIF(PTK_CB_STORAGE_DEBUG)

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
    message(WARNING "No build type specified. Default is Release")
    set(PTK_BUILD_TYPE "Release")
endif()
message(STATUS "pTK build: ${PTK_BUILD_TYPE}")

# OS
if (UNIX AND NOT APPLE) # for Linux, BSD, Solaris, Minix.
    add_definitions(-DPTK_PLATFORM_UNIX)
	set(SKIA_FILENAME "libskia.a")
    set(PTK_PLATFORM "Unix")
    # X11 stuff
    find_package(X11 REQUIRED)
    set(PTK_INCLUDE_DIRS ${X11_INCLUDE_DIR})
    link_directories(${X11_LIBRARIES} ${X11_Xrandr_LIB})
    set(PTK_DEPENDENCIES -ldl -lfreetype -lfontconfig ${X11_LIBRARIES} ${X11_Xrandr_LIB})
    SET(SOME_EXPAT_OPTION OFF CACHE BOOL "Use some expat option")
elseif (WIN32)
    add_definitions(-DPTK_PLATFORM_WINDOWS)
    set(PTK_PLATFORM "Windows")
    set(PTK_OPENGL_DEPENDENCIES "opengl32")
	set(SKIA_FILENAME "skia.lib")
elseif(APPLE)
    add_definitions(-DPTK_PLATFORM_APPLE)
    set(PTK_PLATFORM "Apple")
    set(PTK_DEPENDENCIES "-framework CoreFoundation -framework CoreGraphics -framework CoreText -framework CoreServices -framework AppKit -framework QuartzCore")
    set(PTK_METAL_DEPENDENCIES "-framework Metal")
	set(SKIA_FILENAME "libskia.a")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()
message(STATUS "pTK platform: ${PTK_PLATFORM}")
if (PTK_DEPENDENCIES)
    message(STATUS "pTK dependencies: ${PTK_DEPENDENCIES}")
endif()

# HW Acceleration stuff.
list (FIND PTK_METAL_PLATFORMS ${PTK_PLATFORM} _index)
if (ENABLE_METAL AND ${_index} GREATER -1)
    message(STATUS "Metal backend is enabled")
    add_definitions(-DPTK_METAL)
    add_definitions(-DSK_METAL)
    set(PTK_DEPENDENCIES ${PTK_DEPENDENCIES} ${PTK_METAL_DEPENDENCIES})
else()
    option(ENABLE_OPENGL "Enable OpenGL backend" OFF)
endif ()
list (FIND PTK_OPENGL_PLATFORMS ${PTK_PLATFORM} _index)
if (ENABLE_OPENGL AND ${_index} GREATER -1)
    message(STATUS "OpenGL backend is enabled")
    find_package(OpenGL REQUIRED)
    add_definitions(-DPTK_OPENGL)
    add_definitions(-DSK_GL)
    set(PTK_DEPENDENCIES ${PTK_DEPENDENCIES} ${PTK_OPENGL_DEPENDENCIES})
else()
    SET(ENABLE_OPENGL OFF CACHE BOOL "Enable OpenGL backend" OFF)
endif ()

# Skia properties
set(SKIA_PATH "${CMAKE_SOURCE_DIR}/third_party/skia" CACHE STRING "Path to SKIA directory")
set(SKIA_LIB_PATH "${SKIA_PATH}/out/${PTK_BUILD_TYPE}" CACHE STRING "Path to SKIA lib directory")
set(SKIA_LIB_FILE "${SKIA_LIB_PATH}/${SKIA_FILENAME}")
message(STATUS "Skia include path: ${SKIA_PATH}")
message(STATUS "Skia lib path: ${SKIA_LIB_PATH}")
message(STATUS "Skia lib file: ${SKIA_LIB_FILE}")

# Check if the skia lib file exists.
message(STATUS "Looking for ${SKIA_FILENAME}")
if(EXISTS "${SKIA_LIB_FILE}")
    message(STATUS "FOUND ${SKIA_FILENAME}: TRUE")
else()
    message(STATUS "FOUND ${SKIA_FILENAME}: FALSE")
    message(FATAL_ERROR "Could not find ${SKIA_FILENAME} in ${SKIA_LIB_PATH}")
endif()

# Skia library
add_library(skia STATIC IMPORTED)
set_target_properties(skia PROPERTIES
    IMPORTED_LOCATION "${SKIA_LIB_FILE}"
    INTERFACE_INCLUDE_DIRECTORIES "${SKIA_PATH}"
)

# Threads
find_package(Threads REQUIRED)

# Define library
add_subdirectory(${CMAKE_SOURCE_DIR}/src)

# Example
if (BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/example)
endif (BUILD_EXAMPLES)

# Tests
if (BUILD_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/Catch2)
    set_property(TARGET Catch2 Catch2WithMain PROPERTY
            MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
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
