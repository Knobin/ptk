//
//  util/color.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-17.
//

// Local Headers
#include "ptk/util/color.hpp"
#include "ptk/log.hpp"

// C++ Headers
#include <algorithm>
#include <utility>

namespace pTK
{
    Color::Color()
        : r{0}, g{0}, b{0}, a{255}
    {
    }
    
    Color::Color(uint32_t color)
    {
        set_rgba(color);
    }
    
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : r{red}, g{green}, b{blue}, a{alpha}
    {
        
    }
    
    // Get.
    uint32_t Color::to_int() const
    {
        uint32_t r_value = 0;
        r_value |= (r << 24);
        r_value |= (g << 16);
        r_value |= (b << 8);
        r_value |= a;
        
        return r_value;
    }
    
    // Set.
    void Color::set_rgb(uint8_t red, uint8_t green, uint8_t blue)
    {
        r = red;
        g = green;
        b = blue;
    }
    
    void Color::set_rgb(uint32_t color)
    {
        r = (uint8_t)((color >> 24) & 0xFF);
        g = (uint8_t)((color >> 16) & 0xFF);
        b = (uint8_t)((color >> 8) & 0xFF);
    }
    
    void Color::set_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }
    
    void Color::set_rgba(uint32_t color)
    {
        r = (uint8_t)((color >> 24) & 0xFF);
        g = (uint8_t)((color >> 16) & 0xFF);
        b = (uint8_t)((color >> 8) & 0xFF);
        a = (uint8_t)(color & 0xFF);
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
        new_Color.r = std::min(lhs.r + rhs.r, 255);
        new_Color.g = std::min(lhs.g + rhs.g, 255);
        new_Color.b = std::min(lhs.b + rhs.b, 255);
        new_Color.a = std::min(lhs.a + rhs.a, 255);
        
        return new_Color;
    }
    
    Color operator-(const Color& lhs, const Color& rhs)
    {
        Color new_Color;
        new_Color.r = std::max(lhs.r - rhs.r, 0);
        new_Color.g = std::max(lhs.g - rhs.g, 0);
        new_Color.b = std::max(lhs.b - rhs.b, 0);
        new_Color.a = std::max(lhs.a - rhs.a, 0);
        
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
