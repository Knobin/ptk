//
//  platform/KeyMapInit.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// Local Headers
#include "KeyMapInit.hpp"
#include "Platform.hpp"

namespace pTK::Platform
{
    std::map<int32_t, KeyCode> KeyMapInit()
    {
        return PTK_KEYMAP_INIT_FUNC();
    }

} // namespace pTK::Platform
