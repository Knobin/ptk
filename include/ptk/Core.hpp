//
//  Core.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-09-18.
//

#ifndef PTK_CORE_HPP
#define PTK_CORE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Vec2.hpp"

// C++ Headers
#include <limits>

namespace pTK
{
    // returns x + y if no overflow and max of T if overflow.
    template <typename T>
    constexpr T AddWithoutOverflow(T x, T y) noexcept
    {
        constexpr auto max = std::numeric_limits<T>::max();

        if (x > (max - y))
            return max;

        return x + y;
    }

    template <typename T, typename Rounding>
    constexpr Size Vec2ToSize(const Vec2<T>& v, Rounding func) noexcept
    {
        return {static_cast<Size::value_type>(func(v.x)), static_cast<Size::value_type>(func(v.y))};
    }
} // namespace pTK

#endif // PTK_CORE_HPP
