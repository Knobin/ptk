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
        : m_color{u_color::s_color{255, 0, 0, 0}}
    {
    }
    
    Color::Color(uint32_t color)
    {
        set_color(color);
    }
    
    Color::Color(uint8_t red, uint8_t green, uint8_t blue)
        : m_color{u_color::s_color{255, blue, green, red}}
    {
        
    }
    
    Color::Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
        : m_color{u_color::s_color{alpha, blue, green, red}}
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
        uint32_t r_value = 0;
        r_value |= (get_r() << 24);
        r_value |= (get_g() << 16);
        r_value |= (get_b() << 8);
        r_value |= get_a();
        
        return r_value;
    }
    
    // Set.
    void Color::set_r(uint8_t red)
    {
        m_color.rgba.r = red;
    }
    
    void Color::set_g(uint8_t green)
    {
        m_color.rgba.g = green;
    }
    
    void Color::set_b(uint8_t blue)
    {
        m_color.rgba.b = blue;
    }
    
    void Color::set_a(uint8_t alpha)
    {
        m_color.rgba.a = alpha;
    }
    
    void Color::set_rgb(uint8_t red, uint8_t green, uint8_t blue)
    {
        m_color.rgba.r = red;
        m_color.rgba.g = green;
        m_color.rgba.b = blue;
    }
    
    void Color::set_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha)
    {
        m_color.rgba.r = red;
        m_color.rgba.g = green;
        m_color.rgba.b = blue;
        m_color.rgba.a = alpha;
    }
    
    void Color::set_color(const Color& value)
    {
        m_color.data = value.get_raw();
    }
    
    void Color::set_color(uint32_t color)
    {
        set_r( (uint8_t)((color >> 24) & 0xFF) );
        set_g( (uint8_t)((color >> 16) & 0xFF) );
        set_b( (uint8_t)((color >> 8) & 0xFF) );
        set_a( (uint8_t)(color & 0xFF) );
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
    bool Color::operator==(const Color& rhs) const
    {
        return (get_raw() == rhs.get_raw());
    }
    
    bool Color::operator!=(const Color& rhs) const
    {
        return !(*this == rhs);
    }
}
