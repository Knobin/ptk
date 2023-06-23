//
//  src/platform/WindowHandle.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// Local Headers
#include "../include/Log.hpp"
#include "../include/Assert.hpp"

// pTK Headers
#include "ptk/platform/WindowHandle.hpp"

namespace pTK::Platform
{
    namespace WindowHandleFactoryImpl
    {
        // Function to implement by the platform.
        std::unique_ptr<WindowHandle> Make(WindowBase*, const std::string&, const Size&, const WindowInfo&);
    } // namespace WindowHandleFactoryImpl

    std::unique_ptr<WindowHandle> WindowHandle::Make(WindowBase* winBase, const std::string& name, const Size& size,
                                                     const WindowInfo& flags)
    {
        PTK_ASSERT(winBase, "WindowBase cannot be nullptr");
        auto handle = WindowHandleFactoryImpl::Make(winBase, name, size, flags);
        PTK_ASSERT(handle, "Failed to create ApplicationHandle");

        PTK_INFO("Created WindowHandle");
        return handle;
    }

} // namespace pTK::Platform
