//
//  platform/Platform.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_PLATFORM_HPP
#define PTK_PLATFORM_HPP

#if defined(PTK_PLATFORM_WINDOWS)
#include "win/Platform_win.hpp"
#define PTK_INIT_PLATFORM Platform_win::init
#define PTK_DESTROY_PLATFORM Platform_win::destroy
#include "win/MainWindow_win.hpp"
using MAINWINDOW = pTK::MainWindow_win;
#elif defined(PTK_PLATFORM_UNIX)
// TODO: Implement X11 Window.
#include "glfw/MainWindow_glfw.hpp"
#define PTK_INIT_PLATFORM()
#define PTK_DESTROY_PLATFORM()
using MAINWINDOW = pTK::MainWindow_glfw;
#elif defined(PTK_PLATFORM_APPLE)
// TODO: Implement COCOA Window.
#include "mac/MainWindow_mac.hpp"
#define PTK_INIT_PLATFORM()
#define PTK_DESTROY_PLATFORM()
using MAINWINDOW = pTK::MainWindow_mac;
#else
#error "Unsupported platform!"
#endif

#endif // PTK_PLATFORM_HPP
