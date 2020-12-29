# CMakeLists.txt
# File used to add the platform specific files to pTK.

# MacOS files
set(PLATFORM_FILES_MAC platform/mac/Application_mac.hpp
    platform/mac/Application_mac.mm
    platform/mac/MainWindow_mac.hpp
    platform/mac/MainWindow_mac.mm
    platform/mac/RasterPolicy_mac.hpp
    platform/mac/RasterPolicy_mac.mm)
set(PLATFORM_FILES_MAC_METAL platform/mac/MetalContext_mac.hpp
    platform/mac/MetalContext_mac.mm)

# Windows files
set(PLATFORM_FILES_WIN platform/win/Application_win.cpp
    platform/win/MainWindow_win.cpp
    platform/win/RasterPolicy_win.cpp)
set(PLATFORM_FILES_WIN_OPENGL platform/win/GLContext_win.cpp)

# Unix files
set(PLATFORM_FILES_UNIX platform/unix/Application_unix.cpp
    platform/unix/MainWindow_unix.cpp
    platform/unix/RasterPolicy_unix.cpp)
set(PLATFORM_FILES_UNIX_OPENGL platform/unix/GLContext_unix.cpp)

# Set platform files
set(PLATFORM_FILES platform/Platform.hpp)
if (PTK_PLATFORM STREQUAL "Unix")
    set(PLATFORM_FILES ${PLATFORM_FILES}
        ${PLATFORM_FILES_UNIX})
    if (ENABLE_OPENGL)
        set(PLATFORM_FILES ${PLATFORM_FILES}
            ${PLATFORM_FILES_UNIX_OPENGL})
    endif (ENABLE_OPENGL)
elseif (PTK_PLATFORM STREQUAL "Windows")
    set(PLATFORM_FILES ${PLATFORM_FILES}
        ${PLATFORM_FILES_WIN})
    if (ENABLE_OPENGL)
        set(PLATFORM_FILES ${PLATFORM_FILES}
            ${PLATFORM_FILES_WIN_OPENGL})
    endif (ENABLE_OPENGL)
    message(STATUS "Using Windows backend")
elseif (PTK_PLATFORM STREQUAL "macOS")
    set(PLATFORM_FILES ${PLATFORM_FILES}
        ${PLATFORM_FILES_MAC})
    if (ENABLE_METAL)
        set(PLATFORM_FILES ${PLATFORM_FILES}
            ${PLATFORM_FILES_MAC_METAL})
    endif (ENABLE_METAL)
    message(STATUS "Using macOS backend")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

set(PLATFORM_FILES ${PLATFORM_FILES} PARENT_SCOPE)

# All platform files
set(PLATFORM_FILES_ALL ${PLATFORM_FILES}
    ${PLATFORM_FILES_MAC}
    ${PLATFORM_FILES_MAC_HW}
    ${PLATFORM_FILES_WIN}
    ${PLATFORM_FILES_WIN_HW}
    PARENT_SCOPE)
