//
//  util/Color.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-17.
//

#ifndef PTK_UTIL_COLOR_HPP
#define PTK_UTIL_COLOR_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/core/Types.hpp"

namespace pTK
{
    /** Color class implementation.
     
        r, g, b and a are stored as uint8 (8 bits) in range [0, 255].
        These are stored as public members so they can be used with
        a dot.
        
        These members can also be set and retrieved with uint32 type.
        The Layout for this is: 0xRRGGBBAA

        Functions with a single parameter of uint32, expects the
        aforementioned layout in order to be correct. If the
        function only sets r, g and b, unexpected behaviour will occur.
    */
    class PTK_API Color
    {
    public:
        using value_type = byte;
        using size_type = uint32;

    public:
        /** Constructs Color with default values.
         
            @return  default initialized Color
        */
        constexpr Color()
            : r{0}, g{0}, b{0}, a{255}
        {}
        
        /** Constructs Color with default values with color.
         
            @param color   color with alpha in int form
            @return        Color initialized
        */
        explicit constexpr Color(size_type color)
            : r{0}, g{0}, b{0}, a{255}
        {
            r = static_cast<value_type>((color & 0xFF000000) >> 24);
            g = static_cast<value_type>((color & 0x00FF0000) >> 16);
            b = static_cast<value_type>((color & 0x0000FF00) >> 8);
            a = static_cast<value_type>(color & 0xFF);
        }
        
        /** Constructs Color with default with red, green blue and alpha.
            Default value for alpha is 255, if none is specified.
         
            @param red     amount of red [0, 255]
            @param green   amount of green [0, 255]
            @param blue    amount of blue [0, 255]
            @param alpha   amount of alpha [0, 255]
            @return        Color initialized
        */
        constexpr Color(byte red, byte green, byte blue, byte alpha = 255)
            : r{red}, g{green}, b{blue}, a{alpha}
        {}
        
        /** Get function for retrieving copy of Color in int form.
         
            @return    Copy of Color in int form
        */
        size_type to_int() const;
        
        /** Set function for setting r, g and b.
         
            @param red     amount of red [0, 255]
            @param green   amount of green [0, 255]
            @param blue    amount of blue [0, 255]
        */
        void set_rgb(byte red, byte green, byte blue);
        
        /** Set function for setting r, g and b.
            Takes color in uint32 form.
            This will ignore the 8 first bits, they are reserved to
            alpha in this Color implementation and not used here.
         
            Example:
                - Current to_int() = 0xFFFFFFFF
                - set_rgb(C0C0C000)
                - New to_int() = 0xC0C0C0FF
         
            a will remain unchanged.
         
            @param color   color with alpha in int form
        */
        void set_rgb(size_type color);
        
        /** Set function for setting r, g, b and a.
         
            @param red     amount of red [0, 255]
            @param green   amount of green [0, 255]
            @param blue    amount of blue [0, 255]
            @param alpha   amount of alpha [0, 255]
        */
        void set_rgba(byte red, byte green, byte blue, byte alpha);
        
        /** Set function for setting r, g, b and a.
         
            @param color   color with alpha in int form
        */
        void set_rgba(size_type color);
        
        // Variables.
        value_type r;
        value_type g;
        value_type b;
        value_type a;
    };
    
    // Comparison operators.
    PTK_API bool operator==(const Color& lhs, const Color& rhs);
    PTK_API bool operator!=(const Color& lhs, const Color& rhs);
    
    // Binary arithmetic operators.
    PTK_API Color operator+(const Color& lhs, const Color& rhs);
    PTK_API Color operator-(const Color& lhs, const Color& rhs);
    PTK_API Color& operator+=(Color& lhs, const Color& rhs);
    PTK_API Color& operator-=(Color& lhs, const Color& rhs);
}

#endif // PTK_UTIL_COLOR_HPP
