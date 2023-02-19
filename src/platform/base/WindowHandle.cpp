//
//  src/platform/base/WindowHandle.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// Local Headers
#include "../../Log.hpp"
#include "../../core/Assert.hpp"
#include "../Platform.hpp"

// pTK Headers
#include "ptk/platform/base/WindowHandle.hpp"

namespace pTK::Platform
{
    static std::unique_ptr<WindowHandle> MakePlatformWindowHandle(WindowBase* winBase, const std::string& name,
                                                                  const Size& size, const WindowInfo& flags)
    {
        return std::make_unique<PTK_WINDOW_HANDLE_T>(winBase, name, size, flags);
    }

    std::unique_ptr<WindowHandle> WindowHandle::Make(WindowBase* winBase, const std::string& name, const Size& size,
                                                     const WindowInfo& flags)
    {
        PTK_ASSERT(winBase, "WindowBase cannot be nullptr");
        auto handle = MakePlatformWindowHandle(winBase, name, size, flags);
        PTK_ASSERT(handle, "Failed to create ApplicationHandle");

        PTK_INFO("Created WindowHandle");
        return handle;
    }

} // namespace pTK::Platform
