//
//  util/Color.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-17.
//

// Local Headers
#include "ptk/util/Color.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <algorithm>
#include <utility>

namespace pTK
{
    Color::Color()
        : r{0}, g{0}, b{0}, a{255}
    {
    }
    
    Color::Color(uint color)
    {
        set_rgba(color);
    }
    
    Color::Color(byte red, byte green, byte blue, byte alpha)
        : r{red}, g{green}, b{blue}, a{alpha}
    {
        
    }
    
    // Get.
    uint Color::to_int() const
    {
        uint r_value = 0;
        r_value |= static_cast<uint>(r << 24);
        r_value |= static_cast<uint>(g << 16);
        r_value |= static_cast<uint>(b << 8);
        r_value |= static_cast<uint>(a);
        
        return r_value;
    }
    
    // Set.
    void Color::set_rgb(byte red, byte green, byte blue)
    {
        r = red;
        g = green;
        b = blue;
    }
    
    void Color::set_rgb(uint color)
    {
        r = static_cast<byte>((color >> 24) & 0xFF);
        g = static_cast<byte>((color >> 16) & 0xFF);
        b = static_cast<byte>((color >> 8) & 0xFF);
    }
    
    void Color::set_rgba(byte red, byte green, byte blue, byte alpha)
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }
    
    void Color::set_rgba(uint color)
    {
        r = static_cast<byte>((color >> 24) & 0xFF);
        g = static_cast<byte>((color >> 16) & 0xFF);
        b = static_cast<byte>((color >> 8) & 0xFF);
        a = static_cast<byte>(color & 0xFF);
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
        new_Color.r = static_cast<byte>(std::min(lhs.r + rhs.r, 255));
        new_Color.g = static_cast<byte>(std::min(lhs.g + rhs.g, 255));
        new_Color.b = static_cast<byte>(std::min(lhs.b + rhs.b, 255));
        new_Color.a = static_cast<byte>(std::min(lhs.a + rhs.a, 255));
        
        return new_Color;
    }
    
    Color operator-(const Color& lhs, const Color& rhs)
    {
        Color new_Color;
        new_Color.r = static_cast<byte>(std::max(lhs.r - rhs.r, 0));
        new_Color.g = static_cast<byte>(std::max(lhs.g - rhs.g, 0));
        new_Color.b = static_cast<byte>(std::max(lhs.b - rhs.b, 0));
        new_Color.a = static_cast<byte>(std::max(lhs.a - rhs.a, 0));
        
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
