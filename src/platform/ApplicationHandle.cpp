//
//  src/platform/ApplicationHandle.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// Local Headers
#include "../Log.hpp"
#include "../core/Assert.hpp"

// pTK Headers
#include "ptk/core/Exception.hpp"
#include "ptk/platform/ApplicationHandle.hpp"

namespace pTK::Platform
{
    // ApplicationHandle class static definitions.
    std::unique_ptr<ApplicationHandle> ApplicationHandle::s_handle{nullptr};

    namespace AppFactoryImpl
    {
        // Function to implement by the platform.
        std::unique_ptr<ApplicationHandle> Make(ApplicationBase*, std::string_view);
    } // namespace AppFactoryImpl

    ApplicationHandle* ApplicationHandle::Make(ApplicationBase* appBase, std::string_view name)
    {
        if (s_handle)
            throw ApplicationError("ApplicationHandle already initialized");

        PTK_ASSERT(appBase, "ApplicationBase cannot be nullptr");
        s_handle = AppFactoryImpl::Make(appBase, name);
        PTK_ASSERT(s_handle, "Failed to create ApplicationHandle");

        PTK_INFO("Initialized ApplicationHandle");
        return s_handle.get();
    }

} // namespace pTK::Platform
