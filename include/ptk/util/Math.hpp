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
#include <string>

//
// This file is purely for the need of math functions.
//
// Since in Linux apparently some math functions, like ceilf
// are defined in the global namespace and not in std.
//

namespace pTK::Math
    {
        static inline float ceilf(float arg)
        {
            // This is apparently needed in Linux.
            // Since, ceilf is in the global namespace?!
            // Can't use std::ceilf.

            using namespace std;

            // This will call whatever is defined outside this function.
            return ::ceilf(arg);
        }
    } // namespace pTK

#endif // PTK_UTIL_MATH_HPP
