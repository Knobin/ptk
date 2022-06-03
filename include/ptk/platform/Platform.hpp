//
//  platform/Platform.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_PLATFORM_HPP
#define PTK_PLATFORM_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"

#if defined(PTK_PLATFORM_WINDOWS)
    #include "win/ApplicationHandle_win.hpp"
    using PTK_APPLICATION_HANDLE_T = pTK::ApplicationHandle_win;
    #include "win/WindowHandle_win.hpp"
    using PTK_WINDOW_HANDLE_T = pTK::WindowHandle_win;
    #define PTK_PLATFORM_STR "Windows"
    #include "win/KeyMap_win.hpp"
    #define PTK_KEYMAP_INIT_FUNC pTK::KeyCodeMapInit_win
#elif defined(PTK_PLATFORM_UNIX)
    #include "unix/ApplicationHandle_unix.hpp"
    using PTK_APPLICATION_HANDLE_T = pTK::ApplicationHandle_unix;
    #include "unix/WindowHandle_unix.hpp"
    using PTK_WINDOW_HANDLE_T = pTK::WindowHandle_unix;
    #define PTK_PLATFORM_STR "Unix"
    #include "unix/KeyMap_unix.hpp"
    #define PTK_KEYMAP_INIT_FUNC pTK::KeyCodeMapInit_unix
#elif defined(PTK_PLATFORM_APPLE)
    #include "mac/ApplicationHandle_mac.hpp"
    using PTK_APPLICATION_HANDLE_T = pTK::ApplicationHandle_mac;
    #include "mac/WindowHandle_mac.hpp"
    using PTK_WINDOW_HANDLE_T = pTK::WindowHandle_mac;
    #define PTK_PLATFORM_STR "Apple"
    #include "mac/KeyMap_mac.hpp"
    #define PTK_KEYMAP_INIT_FUNC pTK::KeyCodeMapInit_mac
#else
    #error "Unsupported platform!"
#endif

#endif // PTK_PLATFORM_HPP
