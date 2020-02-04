//
//  util/Size.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

#ifndef PTK_UTIL_SIZE_HPP
#define PTK_UTIL_SIZE_HPP

// Local Headers
#include "ptk/core/Types.hpp"

// C++ Headers
#include <limits>

namespace pTK
{
    /** Size class implementation.

        This class is low level class handling size (width and height).
    */
    class Size
    {
    public:
        using value_type = int32;

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
        Size();

        /** Constructs Size with default values width and height.

            @param width   width
            @param height  height
            @return        default initialized Size
        */
        Size(value_type other_width, value_type other_height);
        ~Size() = default;

        /** Function for setting size.

            @param width   new width
            @param height  new height
        */
        void setSize(value_type other_width, value_type other_height);

        value_type width;
        value_type height;
    };

    // Comparison operators.
    bool operator==(const Size& lhs, const Size& rhs);
    bool operator!=(const Size& lhs, const Size& rhs);
    bool operator<(const Size& lhs, const Size& rhs);
    bool operator>(const Size& lhs, const Size& rhs);
    bool operator<=(const Size& lhs, const Size& rhs);
    bool operator>=(const Size& lhs, const Size& rhs);

    // Binary arithmetic operators.
    Size operator+(const Size& lhs, const Size& rhs);
    Size operator-(const Size& lhs, const Size& rhs);
    Size operator*(const Size& lhs, const Size& rhs);
    Size operator/(const Size& lhs, const Size& rhs);
    Size& operator+=(Size& lhs, const Size& rhs);
    Size& operator-=(Size& lhs, const Size& rhs);
    Size& operator*=(Size& lhs, const Size& rhs);
    Size& operator/=(Size& lhs, const Size& rhs);
}



#endif // PTK_UTIL_SIZE_HPP
