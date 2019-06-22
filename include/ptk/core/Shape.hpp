//
//  core/Shape.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_SHAPE_HPP
#define PTK_CORE_SHAPE_HPP

// Local Headers
#include "ptk/core/Widget.hpp"
#include "ptk/util/Color.hpp"

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
        const Color& getColor() const;
        
        /** Function for setting the Color of the Shape.
         
         @param Color   Shape Color
         */
        void setColor(const Color& color);
        
        /** Function for retrieving the Color of the Shape.
         
         @return    Current Color
         */
        const Color& getOutlineColor() const;
        
        /** Function for setting the Color of the outline.
         
         @param outline_color   outline Color
         */
        void setOutlineColor(const Color& outlineColor);
        
        /** Function for retrieving the thickness of the outline.
         
         @return    outline thickness
         */
        float getOutlineThickness() const;
        
        /** Function for setting the thickness of the outline.
         
         @param outline_thickness   thickness of outline
         */
        void setOutlineThickness(float outlineThickness);
        
    private:
        Color m_color;
        Color m_outlineColor;
        float m_outlineThickness;
    };
    
    // Comparison operators.
    bool operator==(const Shape& lhs, const Shape& rhs);
    bool operator!=(const Shape& lhs, const Shape& rhs);
}

#endif // PTK_CORE_SHAPE_HPP
