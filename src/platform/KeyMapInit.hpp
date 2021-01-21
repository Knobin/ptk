//
//  platform/win/KeyMap_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-18.
//

#ifndef PTK_PLATFORM_KEYMAPINIT_HPP
#define PTK_PLATFORM_KEYMAPINIT_HPP

// Local Headers
#include "Platform.hpp"

namespace pTK
{
    std::map<int32, KeyCode> KeyMapInit()
    {
        return PTK_KEYMAP_INIT_FUNC();
    }

} // namespace pTK

#endif // PTK_PLATFORM_KEYMAPINIT_HPP