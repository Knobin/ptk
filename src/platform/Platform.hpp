//
//  platform/Platform.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_PLATFORM_HPP
#define PTK_PLATFORM_HPP

#if defined(PTK_PLATFORM_WINDOWS)
    #include "win/Application_win.hpp"
    using PTK_APPLICATION_TYPE = pTK::Application_win;
    #include "win/MainWindow_win.hpp"
    using PTK_MAINWINDOW_TYPE = pTK::MainWindow_win;
    #define PTK_PLATFORM_STR "Windows"
    #include "win/KeyMap_win.hpp"
    #define PTK_KEYMAP_INIT_FUNC pTK::KeyCodeMapInit_win
#elif defined(PTK_PLATFORM_UNIX)
    #include "unix/Application_unix.hpp"
    using PTK_APPLICATION_TYPE = pTK::Application_unix;
    #include "unix/MainWindow_unix.hpp"
    using PTK_MAINWINDOW_TYPE = pTK::MainWindow_unix;
    #define PTK_PLATFORM_STR "Unix"
#elif defined(PTK_PLATFORM_APPLE)
    #include "mac/Application_mac.hpp"
    using PTK_APPLICATION_TYPE = pTK::Application_mac;
    #include "mac/MainWindow_mac.hpp"
    using PTK_MAINWINDOW_TYPE = pTK::MainWindow_mac;
    #define PTK_PLATFORM_STR "Apple"
#else
    #error "Unsupported platform!"
#endif

#endif // PTK_PLATFORM_HPP
