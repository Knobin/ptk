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
    /** NonMovable struct implementation.
     
        Struct for forbidding copying.
    */
    struct PTK_API NonCopyable
    {
        /** Constructs NonCopyable with default values.

        */
        NonCopyable() = default;

        /** Deleted Copy Constructor.

        */
        NonCopyable(const NonCopyable&) = delete;

        /** Deleted Copy Assignment operator.

        */
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}

#endif // PTK_UTIL_NONCOPYABLE_HPP
