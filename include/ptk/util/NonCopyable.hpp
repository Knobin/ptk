//
//  util/NonCopyable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-29.
//

#ifndef PTK_UTIL_NONCOPYABLE_HPP
#define PTK_UTIL_NONCOPYABLE_HPP

namespace pTK
{
    /** NonMovable struct implementation.
     
        Struct for forbidding copying.
    */
    struct NonCopyable
    {
        NonCopyable() = default;
        NonCopyable(const NonCopyable&) = delete;
        NonCopyable& operator=(const NonCopyable&) = delete;
    };
}

#endif // PTK_UTIL_NONCOPYABLE_HPP
