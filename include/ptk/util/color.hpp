//
//  util/color.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-17.
//

#ifndef PTK_UTIL_COLOR_HPP
#define PTK_UTIL_COLOR_HPP

// C++ Headers
#include <cstdint>

namespace pTK
{
    class Color
    {
    public:
        Color();
        explicit Color(uint32_t color);
        Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
        virtual ~Color() = default;
        
        // Get.
        Color get_color() const;
        uint32_t to_int() const;
        
        // Set.
        void set_rgb(uint8_t red, uint8_t green, uint8_t blue);
        void set_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
        void set_color(const Color& value);
        void set_color(uint32_t color);
        
        // Variables.
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };
    
    // Comparison operators.
    bool operator==(const Color& lhs, const Color& rhs);
    bool operator!=(const Color& lhs, const Color& rhs);
    
    // Binary arithmetic operators.
    Color operator+(const Color& lhs, const Color& rhs);
    Color operator-(const Color& lhs, const Color& rhs);
    Color& operator+=(Color& lhs, const Color& rhs);
    Color& operator-=(Color& lhs, const Color& rhs);
}

#endif // PTK_UTIL_COLOR_HPP
