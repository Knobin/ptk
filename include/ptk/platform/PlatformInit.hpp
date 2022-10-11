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
    /** Function for creating a map for mapping keycodes to Key.

        Creates the map depending on the function of PTK_KEYMAP_INIT_FUNC.

        @return     KeyMap instance
    */
    PTK_API std::map<int32, Key> KeyMapInit();

    /** Function for creating the application.

        Creates the platform app depending on the type of PTK_APPLICATION_HANDLE_T
        and calls PTK_APPLICATION_HANDLE_T::Init(name) function.
    */
    PTK_API void AppInit(const std::string& name);

    /** Function for destroying the application.

        Destroys the platform app depending on the type of PTK_APPLICATION_HANDLE_T
        and calls PTK_APPLICATION_HANDLE_T::Destroy() function.
    */
    PTK_API void AppDestroy();

    /** Function for retrieving the ApplicationHandle instance.

        Retrieves the instance depending on the type of PTK_APPLICATION_HANDLE_T
        and calls PTK_APPLICATION_HANDLE_T::Instance() function.

        @return     KeyMap instance
    */
    PTK_API ApplicationHandle* AppInstance();

} // namespace pTK

#endif // PTK_PLATFORM_PLATFORMINIT_HPP
