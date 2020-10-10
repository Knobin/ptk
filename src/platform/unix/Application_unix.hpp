//
//  platform/win/Platform_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PLATFORM_UNIX_APPLICATION_HPP
#define PTK_PLATFORM_UNIX_APPLICATION_HPP

// Local Headers
#include "ptk/core/platform/ApplicationBase.hpp"

namespace pTK
{
    /** Application_unix class implementation.

        Base application implementation for Unix.
        This class will be instantiated when the Application is created.
    */
    class Application_unix : public ApplicationBase
    {
    public:
        /** Constructs Application_unix with default values.

            @return        default initialized Application_unix
        */
        Application_unix() = default;

        /** Deconstructor for Application_unix

        */
        virtual ~Application_unix() = default;

        int messageLoop() override;

        /** Function for closing the application should close.

        */
        void close() override;
    };
}

#endif // PTK_PLATFORM_UNIX_APPLICATION_HPP
