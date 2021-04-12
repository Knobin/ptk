//
//  util/Color.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-17.
//

// pTK Headers
#include "ptk/util/Color.hpp"

// C++ Headers
#include <algorithm>

namespace pTK
{
    // Get.
    Color::size_type Color::to_int() const
    {
        size_type r_value = 0;
        r_value |= static_cast<size_type>(r << 24);
        r_value |= static_cast<size_type>(g << 16);
        r_value |= static_cast<size_type>(b << 8);
        r_value |= static_cast<size_type>(a);
        
        return r_value;
    }
    
    // Set.
    void Color::set_rgb(byte red, byte green, byte blue)
    {
        r = red;
        g = green;
        b = blue;
    }
    
    void Color::set_rgb(size_type color)
    {
        r = static_cast<value_type>((color & 0xFF000000) >> 24);
        g = static_cast<value_type>((color & 0x00FF0000) >> 16);
        b = static_cast<value_type>((color & 0x0000FF00) >> 8);
    }
    
    void Color::set_rgba(value_type red, value_type green, value_type blue, value_type alpha)
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }
    
    void Color::set_rgba(size_type color)
    {
        set_rgb(color);
        a = static_cast<value_type>(color & 0xFF);
    }
    
    // Comparison operators.
    bool operator==(const Color& lhs, const Color& rhs)
    {
        return (lhs.to_int() == rhs.to_int());
    }
    
    bool operator!=(const Color& lhs, const Color& rhs)
    {
        return !(lhs == rhs);
    }
    
    // Binary arithmetic operators.
    Color operator+(const Color& lhs, const Color& rhs)
    {
        Color new_Color;
        new_Color.r = static_cast<Color::value_type>(std::min(lhs.r + rhs.r, 255));
        new_Color.g = static_cast<Color::value_type>(std::min(lhs.g + rhs.g, 255));
        new_Color.b = static_cast<Color::value_type>(std::min(lhs.b + rhs.b, 255));
        new_Color.a = static_cast<Color::value_type>(std::min(lhs.a + rhs.a, 255));
        
        return new_Color;
    }
    
    Color operator-(const Color& lhs, const Color& rhs)
    {
        Color new_Color;
        new_Color.r = static_cast<Color::value_type>(std::max(lhs.r - rhs.r, 0));
        new_Color.g = static_cast<Color::value_type>(std::max(lhs.g - rhs.g, 0));
        new_Color.b = static_cast<Color::value_type>(std::max(lhs.b - rhs.b, 0));
        new_Color.a = static_cast<Color::value_type>(std::max(lhs.a - rhs.a, 0));
        
        return new_Color;
    }
    
    Color& operator+=(Color& lhs, const Color& rhs)
    {
        return lhs = lhs + rhs;
    }
    
    Color& operator-=(Color& lhs, const Color& rhs)
    {
        return lhs = lhs - rhs;
    }
}
