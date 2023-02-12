//
//  platform/win/KeyMap_mac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-19.
//

#ifndef PTK_PlATFORM_MAC_KEYMAP_HPP
#define PTK_PlATFORM_MAC_KEYMAP_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK::Platform
{
    PTK_API std::map<int32_t, KeyCode> KeyCodeMapInit_mac();

} // namespace pTK

#endif // PTK_PlATFORM_MAC_KEYMAP_HPP
