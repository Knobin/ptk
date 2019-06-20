//
//  core/shape.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_SHAPE_HPP
#define PTK_CORE_SHAPE_HPP

// Local Headers
#include "ptk/core/widget.hpp"
#include "ptk/util/color.hpp"

namespace pTK
{
    class Shape : public Widget
    {
    public:
        /** Constructs Shape with default values.
         
         @return    default initialized Shape
         */
        Shape();
        virtual ~Shape() = default;
        
        /** Function for retrieving the Color of the Shape.
         
         @return    Current Color
         */
        const Color& get_color() const;
        
        /** Function for setting the Color of the Shape.
         
         @param Color   Shape Color
         */
        void set_color(const Color& color);
        
        /** Function for retrieving the Color of the Shape.
         
         @return    Current Color
         */
        const Color& get_outline_color() const;
        
        /** Function for setting the Color of the outline.
         
         @param outline_color   outline Color
         */
        void set_outline_color(const Color& outline_color);
        
        /** Function for retrieving the thickness of the outline.
         
         @return    outline thickness
         */
        float get_outline_thickness() const;
        
        /** Function for setting the thickness of the outline.
         
         @param outline_thickness   thickness of outline
         */
        void set_outline_thickness(float outline_thickness);
        
    private:
        Color m_color;
        Color m_outline_color;
        float m_outline_thickness;
    };
    
    // Comparison operators.
    bool operator==(const Shape& lhs, const Shape& rhs);
    bool operator!=(const Shape& lhs, const Shape& rhs);
}

#endif // PTK_CORE_SHAPE_HPP
