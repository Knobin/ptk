//
//  core/Exception.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

#ifndef PTK_CORE_EXCEPTION_HPP
#define PTK_CORE_EXCEPTION_HPP

// C++ Headers
#include <stdexcept>

namespace pTK
{
    class Exception : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class PlatformError : public Exception
    {
    public:
        using Exception::Exception;
    };

    class WindowError : public Exception
    {
    public:
        using Exception::Exception;
    };

    class ContextError : public Exception
    {
    public:
        using Exception::Exception;
    };
}

#endif // PTK_CORE_EXCEPTION_HPP