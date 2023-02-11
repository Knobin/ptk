//
//  platform/Platform.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-13.
//

// clang-format off

#ifndef PTK_PLATFORM_HPP
#define PTK_PLATFORM_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"

#if defined(PTK_PLATFORM_WINDOWS)
    #define PTK_PLATFORM_STR "Windows"
#elif defined(PTK_PLATFORM_UNIX)
    #define PTK_PLATFORM_STR "Unix"
#elif defined(PTK_PLATFORM_APPLE)
    #define PTK_PLATFORM_STR "Apple"
#else
    #error "Unsupported platform!"
#endif

#endif // PTK_PLATFORM_HPP

// clang-format on
