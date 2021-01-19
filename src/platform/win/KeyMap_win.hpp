//
//  platform/win/KeyMap_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-18.
//

#ifndef PTK_PlATFORM_WIN_KEYMAP_HPP
#define PTK_PlATFORM_WIN_KEYMAP_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK
{
    std::map<int32, KeyCode> KeyCodeMapInit_win();

} // namespace pTK

#endif // PTK_PlATFORM_WIN_KEYMAP_HPP