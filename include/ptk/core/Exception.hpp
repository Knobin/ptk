//
//  core/Exception.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

#ifndef PTK_CORE_EXCEPTION_HPP
#define PTK_CORE_EXCEPTION_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"

// C++ Headers
#include <stdexcept>

namespace pTK
{
    class PTK_API Exception : public std::runtime_error
    {
    public:
        using runtime_error::runtime_error;
    };

    class PTK_API ApplicationError : public Exception
    {
    public:
        using Exception::Exception;
    };

    class PTK_API PlatformError : public Exception
    {
    public:
        using Exception::Exception;
    };

    class PTK_API WindowError : public Exception
    {
    public:
        using Exception::Exception;
    };

    class PTK_API ContextError : public Exception
    {
    public:
        using Exception::Exception;
    };
} // namespace pTK

#endif // PTK_CORE_EXCEPTION_HPP
