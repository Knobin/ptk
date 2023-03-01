//
//  util/Math.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-12-11.
//

#ifndef PTK_UTIL_MATH_HPP
#define PTK_UTIL_MATH_HPP

// C++ Headers
#include <cmath>
#include <limits>

//
// This file is purely for the need of math functions.
//
// Since in Linux apparently some math functions, like ceilf
// are defined in the global namespace and not in std.
//

namespace pTK::Math
{
    /** Function for adding two values without overflow.

        @param x    start value
        @param y    value to add
        @return     x + y if no overflow and max of T if overflow
    */
    template <typename T>
    constexpr T AddWithoutOverflow(T x, T y) noexcept
    {
        constexpr auto max = std::numeric_limits<T>::max();

        if (x > (max - y))
            return max;

        return x + y;
    }

    static inline float ceilf(float arg)
    {
        // This is apparently needed in Linux.
        // Since, ceilf is in the global namespace?!
        // Can't use std::ceilf.

        using namespace std;

        // This will call whatever is defined outside this function.
        return ::ceilf(arg);
    }
} // namespace pTK::Math

#endif // PTK_UTIL_MATH_HPP
