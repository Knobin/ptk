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
#include "ptk/platform/base/ApplicationHandle.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    PTK_API std::map<int32, KeyCode> KeyMapInit();

    PTK_API void AppInit(const std::string& name);
    PTK_API void AppDestroy();
    PTK_API ApplicationHandle* AppInstance();

} // namespace pTK

#endif // PTK_PLATFORM_PLATFORMINIT_HPP
