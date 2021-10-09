//
//  platform/unix/KeyMap_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

#ifndef PTK_PlATFORM_UNIX_KEYMAP_HPP
#define PTK_PlATFORM_UNIX_KEYMAP_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK
{
    std::map<int32, KeyCode> KeyCodeMapInit_unix();

} // namespace pTK

#endif // PTK_PlATFORM_UNIX_KEYMAP_HPP