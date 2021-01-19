//
//  events/KeyMap.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-18.
//

#ifndef PTK_EVENTS_KEYMAP_HPP
#define PTK_EVENTS_KEYMAP_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK::KeyMap
{
    const std::map<int32, Key>& Map();
    bool KeyExists(Key key);
    Key KeyCodeToKey(int32 keycode);
    int32 KeyToKeyCode(Key key);

} // namespace pTK::KeyMap

#endif // PTK_EVENTS_KEYMAP_HPP