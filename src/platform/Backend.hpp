//
//  platform/Backend.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

#ifndef PTK_PLATFORM_BACKEND_HPP
#define PTK_PLATFORM_BACKEND_HPP

#if defined(PTK_PLATFORM_WINDOWS)
#include "platform/win/WinBackend.hpp"
using BACKEND = pTK::WinBackend;
#elif defined(PTK_PLATFORM_UNIX)
#include "platform/glfw/GLFWBackend.hpp"
using BACKEND = pTK::GLFWBackend;
#elif defined(PTK_PLATFORM_APPLE)
#include "platform/glfw/GLFWBackend.hpp"
using BACKEND = pTK::GLFWBackend;
#else
#error "Unsupported platform!"
#endif

#endif // PTK_PLATFORM_BACKEND_HPP
