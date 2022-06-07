# CMakeLists.txt
# File used to add the platform specific files to pTK.

# MacOS files
set(PTK_PLATFORM_FILES_MAC mac/ApplicationHandle_mac.mm
    mac/WindowHandle_mac.mm
    mac/KeyMap_mac.cpp
    mac/RasterPolicy_mac.mm)
set(PTK_PLATFORM_FILES_MAC_METAL mac/MetalContext_mac.mm)

# Windows files
set(PTK_PLATFORM_FILES_WIN win/ApplicationHandle_win.cpp
    win/KeyMap_win.cpp
    win/WindowHandle_win.cpp
    win/MenuBarUtil_win.cpp
    win/RasterPolicy_win.cpp)
set(PTK_PLATFORM_FILES_WIN_OPENGL win/GLContext_win.cpp)

# Unix files
set(PTK_PLATFORM_FILES_UNIX unix/ApplicationHandle_unix.cpp
    unix/KeyMap_unix.cpp
    unix/WindowHandle_unix.cpp
    unix/RasterPolicy_unix.cpp)
set(PTK_PLATFORM_FILES_UNIX_OPENGL unix/GLContext_unix.cpp)

# Set platform files
set(PTK_PLATFORM_FILES PlatformInit.cpp base/ContextBase.cpp)
if (PTK_PLATFORM STREQUAL "Unix")
    set(PTK_PLATFORM_FILES ${PTK_PLATFORM_FILES} ${PTK_PLATFORM_FILES_UNIX})
    set(PTK_PLATFORM_OPENGL_FILES ${PTK_PLATFORM_OPENGL_FILES} 
        ${PTK_PLATFORM_FILES_UNIX_OPENGL})
    message(STATUS "pTK Window: X11")
elseif (PTK_PLATFORM STREQUAL "Windows")
    set(PTK_PLATFORM_FILES ${PTK_PLATFORM_FILES}
        ${PTK_PLATFORM_FILES_WIN})
    set(PTK_PLATFORM_OPENGL_FILES ${PTK_PLATFORM_OPENGL_FILES}
        ${PTK_PLATFORM_FILES_WIN_OPENGL})
    message(STATUS "pTK Window: win32")
elseif (PTK_PLATFORM STREQUAL "Apple")
    set(PTK_PLATFORM_FILES ${PTK_PLATFORM_FILES}
        ${PTK_PLATFORM_FILES_MAC})
    set(PTK_PLATFORM_METAL_FILES ${PTK_PLATFORM_METAL_FILES}
        ${PTK_PLATFORM_FILES_MAC_METAL})
    message(STATUS "pTK Window: Cocoa")
else()
    message(FATAL_ERROR "Unsupported platform")
endif()

if (PTK_ENABLE_OPENGL)
    set(PTK_PLATFORM_FILES ${PTK_PLATFORM_FILES}
        ${PTK_PLATFORM_OPENGL_FILES})
endif (PTK_ENABLE_OPENGL)

if (PTK_ENABLE_METAL)
    set(PTK_PLATFORM_FILES ${PTK_PLATFORM_FILES}
        ${PTK_PLATFORM_METAL_FILES})
endif (PTK_ENABLE_METAL)

# Platform source files.
list(TRANSFORM PTK_PLATFORM_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(PTK_PLATFORM_FILES ${PTK_PLATFORM_FILES} PARENT_SCOPE)

# All platform files
set(PTK_PLATFORM_FILES_ALL
    ${PTK_PLATFORM_FILES_MAC}
    ${PTK_PLATFORM_FILES_MAC_METAL}
    ${PTK_PLATFORM_FILES_WIN}
    ${PTK_PLATFORM_FILES_WIN_OPENGL}
    ${PTK_PLATFORM_FILES_UNIX}
    ${PTK_PLATFORM_FILES_UNIX_OPENGL})
list(TRANSFORM PTK_PLATFORM_FILES_ALL PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(PTK_PLATFORM_FILES_ALL ${PTK_PLATFORM_FILES_ALL} PARENT_SCOPE)
