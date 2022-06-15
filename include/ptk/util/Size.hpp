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
#include "ptk/core/Types.hpp"

// C++ Headers
#include <limits>

namespace pTK
{
    /** Size class implementation.

        This class is low level class handling size (width and height).
    */
    class PTK_API Size
    {
    public:
        using value_type = uint32;

        struct Limits
        {
            static const value_type Min;
            static const value_type Max;
        };

        static const Size Max;
        static const Size Min;

    public:
        /** Constructs Size with default values.

            @return    default initialized Size
        */
        constexpr Size()
            : width{0}, height{0}
        {}

        /** Constructs Size with default values width and height.

            @param width   width
            @param height  height
            @return        default initialized Size
        */
        constexpr Size(value_type t_width, value_type t_height)
            : width{t_width}, height{t_height}
        {}

        /** Function for setting size.

            @param width   new width
            @param height  new height
        */
        void set(value_type t_width, value_type t_height);

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
}

#endif // PTK_UTIL_SIZE_HPP
