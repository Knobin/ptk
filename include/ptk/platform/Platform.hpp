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

// C++ Headers
#include <string_view>

// clang-format off

#if defined(PTK_PLATFORM_WINDOWS)
    #define PTK_PLATFORM_STR "Windows"
#elif defined(PTK_PLATFORM_UNIX)
    #define PTK_PLATFORM_STR "Unix"
#elif defined(PTK_PLATFORM_APPLE)
    #define PTK_PLATFORM_STR "Apple"
#else
    #error "Unsupported platform!"
#endif

// clang-format on

namespace pTK::Platform
{
    [[nodiscard]] std::string_view GetPlatformString() noexcept;
}

#endif // PTK_PLATFORM_HPP
