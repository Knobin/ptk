//
//  util/Size.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

#ifndef PTK_UTIL_SIZE_HPP
#define PTK_UTIL_SIZE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"

// C++ Headers
#include <cstdint>
#include <limits>

namespace pTK
{
    /** Size class implementation.

        This class is low level class handling size (width and height).
    */
    class PTK_API Size
    {
    public:
        using value_type = uint32_t;

        struct PTK_API Limits
        {
            static const value_type Min;
            static const value_type Max;
        };

        static const Size Max;
        static const Size Min;

    public:
        /** Constructs Size from tx and ty.

            Note: w & h will be casted to Size::value_type.

            @param w    width
            @param h    height
            @return     size from w and h
        */
        template <typename T, typename U>
        static constexpr Size MakeNarrow(T w, U h) noexcept
        {
            return {static_cast<value_type>(w), static_cast<value_type>(h)};
        }

        /** Constructs Size from tx and ty.

            @param w        width
            @param h        height
            @param x        width scaling factor
            @param h        height scaling factor
            @param func     rounding function to use
            @return     size from w and h scaled with x and y
        */
        template <typename T, typename U, typename Rounding>
        static constexpr Size MakeScaled(value_type w, value_type h, T x, U y, Rounding func) noexcept
        {
            return {static_cast<value_type>(func(w * x)), static_cast<value_type>(func(h * y))};
        }

    public:
        /** Constructs Size with default values.

            @return    default initialized Size
        */
        constexpr Size() noexcept
            : width{0},
              height{0}
        {}

        /** Constructs Size with default values width and height.

            @param width   width
            @param height  height
            @return        default initialized Size
        */
        constexpr Size(value_type t_width, value_type t_height) noexcept
            : width{t_width},
              height{t_height}
        {}

        /** Function for setting size.

            @param width   new width
            @param height  new height
        */
        void set(value_type t_width, value_type t_height) noexcept;

        value_type width;
        value_type height;
    };

    // Comparison operators.
    PTK_API bool operator==(const Size& lhs, const Size& rhs);
    PTK_API bool operator!=(const Size& lhs, const Size& rhs);
    PTK_API bool operator<(const Size& lhs, const Size& rhs);
    PTK_API bool operator>(const Size& lhs, const Size& rhs);
    PTK_API bool operator<=(const Size& lhs, const Size& rhs);
    PTK_API bool operator>=(const Size& lhs, const Size& rhs);

    // Binary arithmetic operators.
    PTK_API Size operator+(const Size& lhs, const Size& rhs);
    PTK_API Size operator-(const Size& lhs, const Size& rhs);
    PTK_API Size operator*(const Size& lhs, const Size& rhs);
    PTK_API Size operator/(const Size& lhs, const Size& rhs);
    PTK_API Size& operator+=(Size& lhs, const Size& rhs);
    PTK_API Size& operator-=(Size& lhs, const Size& rhs);
    PTK_API Size& operator*=(Size& lhs, const Size& rhs);
    PTK_API Size& operator/=(Size& lhs, const Size& rhs);
} // namespace pTK

#endif // PTK_UTIL_SIZE_HPP
