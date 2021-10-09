//
//  platform/win/PlatformInit.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-04-03.
//

// Local Headers
#include "ptk/platform/PlatformInit.hpp"
#include "ptk/platform/Platform.hpp"

namespace pTK
{
    std::map<int32, KeyCode> KeyMapInit()
    {
        return PTK_KEYMAP_INIT_FUNC();
    }

    void AppInit(const std::string& name)
    {
        PTK_APPLICATION_TYPE::Init(name);
    }

    void AppDestroy()
    {
        PTK_APPLICATION_TYPE::Destroy();
    }

    ApplicationBase *AppInstance()
    {
        return PTK_APPLICATION_TYPE::Instance();
    }

} // namespace pTK
