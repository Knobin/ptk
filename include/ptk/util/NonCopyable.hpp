//
//  util/NonCopyable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-29.
//

#ifndef PTK_UTIL_NONCOPYABLE_HPP
#define PTK_UTIL_NONCOPYABLE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"

namespace pTK
{
    /** NonMovable class implementation.
     
        Struct for forbidding copying.
    */
    class PTK_API NonCopyable
    {
    public:
        /** Constructs NonCopyable with default values.

        */
        NonCopyable() = default;

        /** Destructor for NonCopyable.

        */
        virtual ~NonCopyable() = default;

        /** Deleted Copy Constructor.

        */
        NonCopyable(const NonCopyable&) = delete;

        /** Deleted Copy Assignment operator.

        */
        NonCopyable& operator=(const NonCopyable&) = delete;

    protected:
        /** Move Constructor.

        */
        NonCopyable(NonCopyable&&) = default;

        /** Move Assignment operator.

        */
        NonCopyable& operator=(NonCopyable&&) = default;
    };
}

#endif // PTK_UTIL_NONCOPYABLE_HPP
