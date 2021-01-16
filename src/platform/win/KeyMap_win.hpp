//
//  platform/win/KeyMap_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-16.
//

#ifndef PTK_PlATFORM_WIN_KEYMAP_HPP
#define PTK_PlATFORM_WIN_KEYMAP_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK::KeyMap_win
{
    const std::map<int32, KeyCode>& Map();
    bool KeyExistsInMap(Key key);
    Key TranslateKeyCodeToKey(int32 code);
    int32 TranslateKeyToKeyCode(Key key);

} // namespace pTK

#endif // PTK_PlATFORM_WIN_KEYMAP_HPP