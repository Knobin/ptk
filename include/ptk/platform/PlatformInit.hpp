//
//  platform/win/PlatformInit.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-18.
//

#ifndef PTK_PLATFORM_PLATFORMINIT_HPP
#define PTK_PLATFORM_PLATFORMINIT_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK
{
    /** Function for creating a map for mapping keycodes to Key.

        Creates the map depending on the function of PTK_KEYMAP_INIT_FUNC.

        @return     KeyMap instance
    */
    PTK_API std::map<int32_t, Key> KeyMapInit();

} // namespace pTK

#endif // PTK_PLATFORM_PLATFORMINIT_HPP
