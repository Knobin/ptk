//
//  platform/win/PlatformInit.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-18.
//

#ifndef PTK_PLATFORM_PLATFORMINIT_HPP
#define PTK_PLATFORM_PLATFORMINIT_HPP

// pTK Headers
#include "ptk/platform/base/ApplicationHandle.hpp"

namespace pTK
{
    std::map<int32, KeyCode> KeyMapInit();

    void AppInit(const std::string& name);
    void AppDestroy();
    ApplicationHandle *AppInstance();

} // namespace pTK

#endif // PTK_PLATFORM_PLATFORMINIT_HPP
