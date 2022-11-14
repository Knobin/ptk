//
//  platform/win/PlatformInit.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

// Local Headers
#include "ptk/platform/PlatformInit.hpp"
#include "ptk/platform/Platform.hpp"

namespace pTK
{
    std::map<int32_t, KeyCode> KeyMapInit()
    {
        return PTK_KEYMAP_INIT_FUNC();
    }

} // namespace pTK
