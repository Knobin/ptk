//
//  platform/unix/KeyMap_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

#ifndef PTK_PLATFORM_UNIX_KEYMAP_HPP
#define PTK_PLATFORM_UNIX_KEYMAP_HPP

// pTK Headers
#include "ptk/events/KeyCodes.hpp"

// C++ Headers
#include <map>

namespace pTK::Platform
{
    PTK_API std::map<int32_t, KeyCode> KeyCodeMapInit_unix();

} // namespace pTK::Platform

#endif // PTK_PLATFORM_UNIX_KEYMAP_HPP
