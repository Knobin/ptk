//
//  src/platform/Platform.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-21.
//

// pTK Headers
#include "ptk/platform/Platform.hpp"

namespace pTK::Platform
{
    std::string_view GetPlatformString() noexcept
    {
        return PTK_PLATFORM_STR;
    }
} // namespace pTK::Platform
