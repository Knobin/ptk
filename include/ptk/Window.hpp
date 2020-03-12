//
//  Window.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-04.
//

#ifndef PTK_WINDOW_HPP
#define PTK_WINDOW_HPP

#if defined(PTK_PLATFORM_WINDOWS)
#include "ptk/platform/win32/Win32Window.hpp"
#elif defined(PTK_PLATFORM_UNIX)
#pragma message("X11 or Wayland is not implemented, using GLFW with OpenGL")
#include "ptK/platform/Window.hpp"
#elif defined(PTK_PLATFORM_APPLE)
#pragma message("Cocoa is not implemented, using GLFW with OpenGL")
#include "ptK/platform/Window.hpp"
#else
#error "Unsupported platform!"
#endif

#endif // PTK_WINDOW_HPP