# CMakeLists.txt
# File used to add the platform specific files to pTK.

# Core files
set(PTK_PLATFORM_CORE_FILES Platform.cpp
    KeyMapFactory.hpp
    KeyMapFactory.cpp
    ApplicationHandle.cpp
    ContextFactory.cpp
    WindowHandle.cpp
    RasterContext.cpp)

# MacOS files
set(PTK_PLATFORM_FILES_MAC mac/ApplicationHandleMac.hpp
    mac/ApplicationHandleMac.mm
    mac/WindowHandleMac.hpp
    mac/WindowHandleMac.mm
    mac/KeyMapMac.cpp
    mac/RasterContextMac.hpp
    mac/RasterContextMac.mm
    mac/ContextFactoryMac.mm)
set(PTK_PLATFORM_FILES_MAC_METAL mac/MetalContextMac.hpp mac/MetalContextMac.mm)

# Windows files
set(PTK_PLATFORM_FILES_WIN win/ApplicationHandleWin.hpp
    win/ApplicationHandleWin.cpp
    win/KeyMapWin.cpp
    win/WindowHandleWin.hpp
    win/WindowHandleWin.cpp
    win/MenuBarUtilWin.hpp
    win/MenuBarUtilWin.cpp
    win/RasterContextWin.hpp
    win/RasterContextWin.cpp
    win/ContextFactoryWin.cpp)
set(PTK_PLATFORM_FILES_WIN_OPENGL win/GLContextWin.hpp win/GLContextWin.cpp)

# Unix files
set(PTK_PLATFORM_FILES_UNIX unix/ApplicationHandleUnix.hpp
    unix/ApplicationHandleUnix.cpp
    unix/KeyMapUnix.cpp
    unix/WindowHandleUnix.hpp
    unix/WindowHandleUnix.cpp
    unix/RasterContextUnix.hpp
    unix/RasterContextUnix.cpp
    unix/ContextFactoryUnix.cpp
    unix/x11.hpp)
set(PTK_PLATFORM_FILES_UNIX_OPENGL unix/GLContextUnix.hpp unix/GLContextUnix.cpp)

# Set platform files
set(PTK_PLATFORM_FILES ${PTK_PLATFORM_CORE_FILES})
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
    ${PTK_PLATFORM_CORE_FILES}
    ${PTK_PLATFORM_FILES_MAC}
    ${PTK_PLATFORM_FILES_MAC_METAL}
    ${PTK_PLATFORM_FILES_WIN}
    ${PTK_PLATFORM_FILES_WIN_OPENGL}
    ${PTK_PLATFORM_FILES_UNIX}
    ${PTK_PLATFORM_FILES_UNIX_OPENGL})
list(TRANSFORM PTK_PLATFORM_FILES_ALL PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(PTK_PLATFORM_FILES_ALL ${PTK_PLATFORM_FILES_ALL} PARENT_SCOPE)
