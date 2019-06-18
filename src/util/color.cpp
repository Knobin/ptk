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
#include <utility>

namespace pTK
{
    uint8_t add(uint8_t base, u_int8_t to_add)
    {
        if ( !((base + to_add) > 255) )
            return base + to_add;
        
        PTK_WARN("[Color] add overflow");
        return 255;
    }
    
    uint8_t sub(uint8_t base, u_int8_t to_sub)
    {
        if ( !((base - to_sub) < 0) )
            return base - to_sub;
        
        PTK_WARN("[Color] sub underflow");
        return 0;
    }
    
    Color::Color()
        : r{0}, g{0}, b{0}, a{255}
    {
    }
    
    Color::Color(uint32_t color)
    {
        set_color(color);
    }
    
    Color::Color(uint8_t red, uint8_t green, uint8_t blue)
        : r{red}, g{green}, b{blue}, a{255}
    {
        
    }
    
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : r{red}, g{green}, b{blue}, a{alpha}
    {
        
    }
    
    // Copy.
    Color::Color(const Color& rhs)
    {
        set_color(rhs.get_raw());
    }
    
    // Assignment.
    Color& Color::operator=(Color rhs)
    {
        std::swap(r, rhs.r);
        std::swap(g, rhs.g);
        std::swap(b, rhs.b);
        std::swap(a, rhs.a);
        
        return *this;
    }
    
    // Get.
    Color Color::get_color() const
    {
        return *this;
    }
    
    uint32_t Color::get_raw() const
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
    
    void Color::set_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    {
        r = red;
        g = green;
        b = blue;
        a = alpha;
    }
    
    void Color::set_color(const Color& value)
    {
        set_rgba(value.r, value.g, value.b, value.a);
    }
    
    void Color::set_color(uint32_t color)
    {
        r = (uint8_t)((color >> 24) & 0xFF);
        g = (uint8_t)((color >> 16) & 0xFF);
        b = (uint8_t)((color >> 8) & 0xFF);
        a = (uint8_t)(color & 0xFF);
    }
    
    // Binary arithmetic operators.
    Color Color::operator+(const Color& rhs) const
    {
        Color new_Color{*this};
        
        // New Colors.
        new_Color.r = add(new_Color.r, rhs.r);
        new_Color.g = add(new_Color.g, rhs.g);
        new_Color.b = add(new_Color.b, rhs.b);
        new_Color.a = add(new_Color.a, rhs.a);
        
        return new_Color;
    }
    
    Color& Color::operator+=(const Color& rhs)
    {
        // New Colors.
        r = add(r, rhs.r);
        g = add(g, rhs.g);
        b = add(b, rhs.b);
        a = add(a, rhs.a);
        
        return *this;
    }
    
    Color Color::operator-(const Color& rhs) const
    {
        Color new_Color{*this};
        
        // New Colors.
        new_Color.r = sub(new_Color.r, rhs.r);
        new_Color.g = sub(new_Color.g, rhs.g);
        new_Color.b = sub(new_Color.b, rhs.b);
        new_Color.a = sub(new_Color.a, rhs.a);
        
        return new_Color;
    }
    
    Color& Color::operator-=(const Color& rhs)
    {
        // New Colors.
        r = sub(r, rhs.r);
        g = sub(g, rhs.g);
        b = sub(b, rhs.b);
        a = sub(a, rhs.a);
        
        return *this;
    }
    
    // Comparison operators.
    bool Color::operator==(const Color& rhs) const
    {
        return (get_raw() == rhs.get_raw());
    }
    
    bool Color::operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }
}
