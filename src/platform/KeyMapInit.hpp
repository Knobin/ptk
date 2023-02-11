//
//  platform/KeyMapInit.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

#ifndef PTK_PLATFORM_KEYMAPINIT_HPP
#define PTK_PLATFORM_KEYMAPINIT_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK::Platform
{
    /** Function for creating a map for mapping keycodes to Key.
        Creates the map depending on the function of PTK_KEYMAP_INIT_FUNC.
        @return     KeyMap instance
    */
    PTK_API std::map<int32_t, Key> KeyMapInit();

} // namespace pTK::Platform

#endif // PTK_PLATFORM_KEYMAPINIT_HPP
