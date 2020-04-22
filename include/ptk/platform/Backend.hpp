//
//  platform/Backend.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_PLATFORM_BACKEND_HPP
#define PTK_PLATFORM_BACKEND_HPP

#if defined(PTK_PLATFORM_WINDOWS)
#include "ptk/platform/win/WinBackend.hpp"
using BACKEND = pTK::WinBackend;
#elif defined(PTK_PLATFORM_UNIX)
#include "ptk/platform/glfw/GLFWBackend.hpp"
using BACKEND = pTK::GLFWBackend;
#elif defined(PTK_PLATFORM_APPLE)
#include "ptk/platform/glfw/GLFWBackend.hpp"
using BACKEND = pTK::GLFWBackend;
#else
#error "Unsupported platform!"
#endif

#endif // PTK_PLATFORM_BACKEND_HPP
