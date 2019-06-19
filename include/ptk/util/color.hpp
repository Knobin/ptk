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
    /** Color class implementation.
     
     r, g, b and a are stored as uint8 (8 bits) in range [0, 255].
     These are stored as public members so they can be used with
     a dot.
     
     These members can also be set and retrieved with uint32 type.
     The Layout for this is: 0xRRGGBBAA

     Functions with a single parameter of uint32, expects the
     aforementioned layout in order to be correct. If the
     function only sets r, g and b, and expects a uint32,
     the AA bits are ignored.
     */
    class Color
    {
    public:
        /** Constructs Color with default values.
         
         @return  default initialized Color
         */
        Color();
        
        /** Constructs Color with default values with color.
         
         @param color   color with alpha in int form
         @return        Color initialized
         */
        explicit Color(uint32_t color);
        
        /** Constructs Color with default with red, green blue and alpha.
            Default value for alpha is 255, if none is specified.
         
         @param red     amount of red [0, 255]
         @param green   amount of green [0, 255]
         @param blue    amount of blue [0, 255]
         @param alpha   amount of alpha [0, 255]
         @return        Color initialized
         */
        Color(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha = 255);
        virtual ~Color() = default;
        
        /** Get function for retrieving copy of Color in int form.
         
         @return    Copy of Color in int form
         */
        uint32_t to_int() const;
        
        /** Set function for setting r, g and b.
         
         @param red     amount of red [0, 255]
         @param green   amount of green [0, 255]
         @param blue    amount of blue [0, 255]
         */
        void set_rgb(uint8_t red, uint8_t green, uint8_t blue);
        
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
        void set_rgb(uint32_t color);
        
        /** Set function for setting r, g, b and a.
         
         @param red     amount of red [0, 255]
         @param green   amount of green [0, 255]
         @param blue    amount of blue [0, 255]
         @param alpha   amount of alpha [0, 255]
         */
        void set_rgba(uint8_t red, uint8_t green, uint8_t blue, uint8_t alpha);
        
        /** Set function for setting r, g, b and a.
         
         @param color   color with alpha in int form
         */
        void set_rgba(uint32_t color);
        
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
