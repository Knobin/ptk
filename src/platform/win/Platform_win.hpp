//
//  platform/win/Platform_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

#ifndef PTK_PLATFORM_WIN_PLATFORM_HPP
#define PTK_PLATFORM_WIN_PLATFORM_HPP

// C++ Headers
#include <string>

namespace pTK
{
    namespace Platform_win
    {
        /** Function for initializing the Windows platform.

            This will be called from the Application constructor.
            For any Windows specific things need to be initialized/created.
        */
        void init();

        /** Function for destroying the Windows platform.

            This will be called from the Application destructor.
            For any Windows specific things need to be destroyed.
        */
        void destroy();

        /** Function for converting std::string to UTF16 std::stringw.

            @param str  string to convert
            @return     UTF16 string
        */
        std::wstring stringToUTF16(const std::string& str);
    };
}

#endif // PTK_PLATFORM_WIN_PLATFORM_HPP
