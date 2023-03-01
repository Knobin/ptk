cmake_minimum_required(VERSION 3.15.7)
cmake_policy(SET CMP0091 NEW)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
project(ptk VERSION 1 DESCRIPTION "C++ library for building user interfaces.")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)

# General options
option(PTK_BUILD_TESTS "Build test programs" OFF)
option(PTK_BUILD_EXAMPLES "Build example programs" ON)

# Hardware acceleration options.
option(PTK_ENABLE_OPENGL "Enable OpenGL backend" ON)
option(PTK_ENABLE_METAL "Enable Metal backend" ON)

# Available HW on platforms.
set(PTK_OPENGL_PLATFORMS "Windows" "Unix")
set(PTK_METAL_PLATFORMS "Apple")

# Enable building with AddressSanitizer.
option(PTK_ENABLE_SANITIZE "Enable AddressSanitizer" OFF)
if (PTK_ENABLE_SANITIZE)
    add_compile_options(-fsanitize=address -fsanitize-recover=all)
    add_link_options(-fsanitize=address -fsanitize-recover=all)
endif(PTK_ENABLE_SANITIZE)

# Specific Debug Defines
option(PTK_CB_STORAGE_DEBUG "Enable Callback Storage debug printing" OFF)
if (PTK_CB_STORAGE_DEBUG)
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_CB_STORAGE_DEBUG)
endif(PTK_CB_STORAGE_DEBUG)

# Compiler
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_COMPILER_CLANG)
    message(STATUS "Compiler: Clang")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_COMPILER_GCC)
    message(STATUS "Compiler: GCC")
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_COMPILER_MSVC)
    message(STATUS "Compiler: MSVC")
endif()

# Build Type
if (CMAKE_BUILD_TYPE MATCHES Debug)
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_DEBUG)
    set(PTK_BUILD_TYPE "Debug")
elseif (CMAKE_BUILD_TYPE MATCHES Release)
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_RELEASE)
    set(PTK_BUILD_TYPE "Release")
else()
    message(WARNING "No build type specified. Default is Release")
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_RELEASE)
    set(PTK_BUILD_TYPE "Release")
endif()

# OS
if (UNIX AND NOT APPLE) # for Linux, BSD, Solaris, Minix.
    set(SKIA_FILENAME "libskia.a")
    set(PTK_PLATFORM "Unix")
    # X11 stuff
    find_package(X11 REQUIRED)
    set(PTK_INCLUDE_DIRS ${X11_INCLUDE_DIR})
    link_directories(${X11_LIBRARIES} ${X11_Xrandr_LIB})
    set(PTK_DEPENDENCIES -ldl -lfreetype -lfontconfig ${X11_LIBRARIES} ${X11_Xrandr_LIB})
    set(PTK_OPENGL_DEPENDENCIES "OpenGL::GL")
elseif (WIN32)
    set(PTK_PLATFORM "Windows")
    set(PTK_OPENGL_DEPENDENCIES "opengl32")
	set(SKIA_FILENAME "skia.lib")
elseif(APPLE)
    set(PTK_PLATFORM "Apple")
    set(PTK_DEPENDENCIES "-framework CoreFoundation -framework CoreGraphics -framework CoreText -framework CoreServices -framework AppKit -framework QuartzCore")
    set(PTK_METAL_DEPENDENCIES "-framework Metal")
	set(SKIA_FILENAME "libskia.a")
else()
    message(FATAL_ERROR "Unsupported platform!")
endif()

# Enable Metal rendering backend if on supported platform and PTK_ENABLE_METAL=ON
list (FIND PTK_METAL_PLATFORMS ${PTK_PLATFORM} _index)
if (PTK_ENABLE_METAL AND ${_index} GREATER -1)
    message(STATUS "Metal backend is enabled")
    set(PTK_USE_HW TRUE)
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_METAL)
    set(SKIA_DEFINITIONS ${SKIA_DEFINITIONS} SK_METAL)
    set(PTK_DEPENDENCIES ${PTK_DEPENDENCIES} ${PTK_METAL_DEPENDENCIES})
endif ()

# Enable OpenGL rendering backend if on supported platform and PTK_ENABLE_OPENGL=ON
list (FIND PTK_OPENGL_PLATFORMS ${PTK_PLATFORM} _index)
if (PTK_ENABLE_OPENGL AND ${_index} GREATER -1)
    message(STATUS "OpenGL backend is enabled")
    set(PTK_USE_HW TRUE)
    find_package(OpenGL REQUIRED)
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_OPENGL)
    set(SKIA_DEFINITIONS ${SKIA_DEFINITIONS} SK_GL)
    set(PTK_DEPENDENCIES ${PTK_DEPENDENCIES} ${PTK_OPENGL_DEPENDENCIES})
endif ()

# Set things for release build.
if (PTK_BUILD_TYPE MATCHES Release)
    set(SKIA_DEFINITIONS ${SKIA_DEFINITIONS} SK_RELEASE)
    if (PTK_USE_HW)
        set(SKIA_DEFINITIONS ${SKIA_DEFINITIONS} SK_SUPPORT_GPU=1)
    endif()
endif()

# Set things for debug build.
if (PTK_BUILD_TYPE MATCHES Debug)
    set(SKIA_DEFINITIONS ${SKIA_DEFINITIONS} SK_DEBUG)
    if (PTK_USE_HW)
        set(SKIA_DEFINITIONS ${SKIA_DEFINITIONS} SK_SUPPORT_GPU=1 GR_TEST_UTILS=1)
    endif()
endif()

# Library build prints.
message(STATUS "pTK build: ${PTK_BUILD_TYPE}")
if (BUILD_SHARED_LIBS)
    set(PTK_DEFINITIONS ${PTK_DEFINITIONS} PTK_SHARED)
    set(PTK_EXPORTS_DEFINE PTK_EXPORTS)
    message(STATUS "pTK library type: SHARED")
    message(WARNING "Building pTK as shared library is experimental")
else()
    message(STATUS "pTK library type: STATIC")
endif()

# Print platform and dependencies.
message(STATUS "pTK platform: ${PTK_PLATFORM}")
if (PTK_DEFINITIONS)
    message(STATUS "pTK definitions: ${PTK_DEFINITIONS}")
endif()
if (PTK_DEPENDENCIES)
    message(STATUS "pTK dependencies: ${PTK_DEPENDENCIES}")
endif()

# Skia properties
set(SKIA_INCLUDE_PATH "${CMAKE_SOURCE_DIR}/third_party/skia" CACHE STRING "Path to SKIA directory")
set(SKIA_LIBRARY_LOCATION "${SKIA_INCLUDE_PATH}/out/${PTK_BUILD_TYPE}/${SKIA_FILENAME}" CACHE STRING "SKIA library file location")
message(STATUS "Skia include path: ${SKIA_INCLUDE_PATH}")
message(STATUS "Skia library file: ${SKIA_LIBRARY_LOCATION}")
if (SKIA_DEFINITIONS)
    message(STATUS "Skia definitions: ${SKIA_DEFINITIONS}")
endif()

# Check if the skia lib file exists.
message(STATUS "Looking for ${SKIA_FILENAME}")
if(EXISTS "${SKIA_LIBRARY_LOCATION}")
    message(STATUS "FOUND ${SKIA_FILENAME}: TRUE")
else()
    message(STATUS "FOUND ${SKIA_FILENAME}: FALSE")
    message(FATAL_ERROR "Could not find ${SKIA_FILENAME} at ${SKIA_LIBRARY_LOCATION}")
endif()

# Skia library
add_library(skia STATIC IMPORTED)
set_target_properties(skia PROPERTIES
    IMPORTED_LOCATION "${SKIA_LIBRARY_LOCATION}"
    INTERFACE_INCLUDE_DIRECTORIES "${SKIA_INCLUDE_PATH}")

# Threads
find_package(Threads REQUIRED)

# Header files
file(GLOB_RECURSE PTK_HEADER_FILES ${CMAKE_SOURCE_DIR}/include/ptk/*.hpp)
source_group(TREE "${CMAKE_SOURCE_DIR}/include/ptk" PREFIX "pTK Header Files" FILES ${PTK_HEADER_FILES})

# Get library source files
add_subdirectory(${CMAKE_SOURCE_DIR}/src)
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "pTK Source Files" FILES ${PTK_SOURCE_FILES_ALL})

# Define library
add_library(${PROJECT_NAME} ${PTK_HEADER_FILES} ${PTK_SOURCE_FILES})

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)

# includes and deps
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include" ${PTK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PTK_DEPENDENCIES} skia Threads::Threads)
target_compile_definitions(${PROJECT_NAME} PRIVATE ${PTK_DEFINITIONS} ${PTK_EXPORTS_DEFINE} ${SKIA_DEFINITIONS})

# OpenGL
if (ENABLE_OPENGL)
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif (ENABLE_OPENGL)

# spdlog
if (PTK_BUILD_TYPE MATCHES Debug)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/spdlog/include)
endif()

# Compiler flags for ptk
if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /permissive- /W4 /w14640)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC
            -Wall -Wextra -Wpedantic -fvisibility=hidden -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align
            -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wdouble-promotion -Wformat=2)
endif ()

# Example
if (PTK_BUILD_EXAMPLES)
    add_subdirectory(${CMAKE_SOURCE_DIR}/example)
endif (PTK_BUILD_EXAMPLES)

# Tests
if (PTK_BUILD_TESTS)
    add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/Catch2)
    enable_testing()
    add_subdirectory(${CMAKE_SOURCE_DIR}/tests)
endif (PTK_BUILD_TESTS)

# Print options
message(STATUS "Build Tests: ${PTK_BUILD_TESTS}")
message(STATUS "Build Examples: ${PTK_BUILD_EXAMPLES}")
message(STATUS "AddressSanitizer is enabled: ${PTK_ENABLE_SANITIZE}")
