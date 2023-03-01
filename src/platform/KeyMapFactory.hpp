//
//  platform/KeyMapFactory.hpp
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
    class KeyMapFactory
    {
    public:
        /** Function for creating a map for mapping keycodes to Key.

            @return     KeyMap instance
        */
        PTK_API static std::map<int32_t, Key> Make();
    };

} // namespace pTK::Platform

#endif // PTK_PLATFORM_KEYMAPINIT_HPP
