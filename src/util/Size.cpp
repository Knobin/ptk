//
//  util/Size.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

// pTK Headers
#include "ptk/util/Size.hpp"

namespace pTK
{
    const Size::value_type Size::Limits::Min = 0;
    const Size::value_type Size::Limits::Max = std::numeric_limits<Size::value_type>::max();

    const Size Size::Min(Size::Limits::Min, Size::Limits::Min);
    const Size Size::Max(Size::Limits::Max, Size::Limits::Max);

    Size::Size()
        : width{Limits::Min}, height{Limits::Min}
    {

    }

    Size::Size(value_type other_width, value_type other_height)
        : width{other_width}, height{other_height}
    {

    }

    void Size::setSize(value_type other_width, value_type other_height)
    {
        width = other_width;
        height = other_height;
    }

    // Comparison operators.
    bool operator==(const Size& lhs, const Size& rhs)
    {
        return ((lhs.width == rhs.width)
                && (lhs.height == rhs.height));
    }

    bool operator!=(const Size& lhs, const Size& rhs)
    {
        return !(lhs == rhs);
    }

    bool operator<(const Size& lhs, const Size& rhs)
    {
        return rhs > lhs;
    }

    bool operator>(const Size& lhs, const Size& rhs)
    {
        return ((lhs.width > rhs.width)
                && (lhs.height > rhs.height));
    }

    bool operator<=(const Size& lhs, const Size& rhs)
    {
        return !(lhs > rhs);
    }

    bool operator>=(const Size& lhs, const Size& rhs)
    {
        return !(lhs < rhs);
    }

    // Binary arithmetic operators.
    Size operator+(const Size& lhs, const Size& rhs)
    {
        Size newSize;
        newSize.width = lhs.width + rhs.width;
        newSize.height = lhs.height + rhs.height;

        return newSize;
    }

    Size operator-(const Size& lhs, const Size& rhs)
    {
        Size newSize;
        newSize.width = lhs.width - rhs.width;
        newSize.height = lhs.height - rhs.height;

        return newSize;
    }

    Size operator*(const Size& lhs, const Size& rhs)
    {
        Size newSize;
        newSize.width = lhs.width * rhs.width;
        newSize.height = lhs.height * rhs.height;

        return newSize;
    }

    Size operator/(const Size& lhs, const Size& rhs)
    {
        Size newSize;
        newSize.width = lhs.width / rhs.width;
        newSize.height = lhs.height / rhs.height;

        return newSize;
    }

    Size& operator+=(Size& lhs, const Size& rhs)
    {
        return lhs = lhs + rhs;
    }

    Size& operator-=(Size& lhs, const Size& rhs)
    {
        return lhs = lhs - rhs;
    }

    Size& operator*=(Size& lhs, const Size& rhs)
    {
        return lhs = lhs * rhs;
    }

    Size& operator/=(Size& lhs, const Size& rhs)
    {
        return lhs = lhs / rhs;
    }
}
