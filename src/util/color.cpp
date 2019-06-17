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
        : m_color{u_color::s_color{0, 0, 0, 255}}
    {
    }
    
    Color::Color(uint32_t color)
    {
        set_color(color);
    }
    
    Color::Color(uint8_t red, uint8_t green, uint8_t blue)
        : m_color{u_color::s_color{red, green, blue, 255}}
    {
        
    }
    
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : m_color{u_color::s_color{red, green, blue, alpha}}
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
        std::swap(m_color.data, rhs.m_color.data);
        return *this;
    }
    
    // Get.
    uint8_t Color::get_r() const
    {
        return m_color.rgba.r;
    }
    
    uint8_t Color::get_g() const
    {
        return m_color.rgba.g;
    }
    
    uint8_t Color::get_b() const
    {
        return m_color.rgba.b;
    }
    
    uint8_t Color::get_a() const
    {
        return m_color.rgba.a;
    }
    
    Color Color::get_color() const
    {
        return *this;
    }
    
    uint32_t Color::get_raw() const
    {
        return m_color.data;
    }
    
    // Set.
    void Color::set_r(uint8_t red)
    {
        m_color.rgba.r = red;
    }
    
    void Color::set_g(uint8_t green)
    {
        m_color.rgba.r = green;
    }
    
    void Color::set_b(uint8_t blue)
    {
        m_color.rgba.r = blue;
    }
    
    void Color::set_a(uint8_t alpha)
    {
        m_color.rgba.r = alpha;
    }
    
    void Color::set_color(const Color& value)
    {
        m_color.data = value.get_raw();
    }
    
    void Color::set_color(uint32_t color)
    {
        m_color.data = color;
    }
    
    // Binary arithmetic operators.
    Color Color::operator+(const Color& rhs) const
    {
        Color new_Color{*this};
        
        // New Colors.
        new_Color.m_color.rgba.r = add(new_Color.get_r(), rhs.get_r());
        new_Color.m_color.rgba.g = add(new_Color.get_g(), rhs.get_g());
        new_Color.m_color.rgba.b = add(new_Color.get_b(), rhs.get_b());
        new_Color.m_color.rgba.a = add(new_Color.get_a(), rhs.get_a());
        
        return new_Color;
    }
    
    Color& Color::operator+=(const Color& rhs)
    {
        // New Colors.
        m_color.rgba.r = add(get_r(), rhs.get_r());
        m_color.rgba.g = add(get_g(), rhs.get_g());
        m_color.rgba.b = add(get_b(), rhs.get_b());
        m_color.rgba.a = add(get_a(), rhs.get_a());
        
        return *this;
    }
    
    Color Color::operator-(const Color& rhs) const
    {
        Color new_Color{*this};
        
        // New Colors.
        new_Color.m_color.rgba.r = sub(new_Color.get_r(), rhs.get_r());
        new_Color.m_color.rgba.g = sub(new_Color.get_g(), rhs.get_g());
        new_Color.m_color.rgba.b = sub(new_Color.get_b(), rhs.get_b());
        new_Color.m_color.rgba.a = sub(new_Color.get_a(), rhs.get_a());
        
        return new_Color;
    }
    
    Color& Color::operator-=(const Color& rhs)
    {
        // New Colors.
        m_color.rgba.r = sub(get_r(), rhs.get_r());
        m_color.rgba.g = sub(get_g(), rhs.get_g());
        m_color.rgba.b = sub(get_b(), rhs.get_b());
        m_color.rgba.a = sub(get_a(), rhs.get_a());
        
        return *this;
    }
    
    // Comparison operators.
    bool Color::operator==(const Color& rhs)
    {
        return (get_raw() == rhs.get_raw());
    }
    
    bool Color::operator!=(const Color& rhs)
    {
        return !(*this == rhs);
    }
}
