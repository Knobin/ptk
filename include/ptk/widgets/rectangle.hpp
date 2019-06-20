//
//  widgets/rectangle.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_WIDGETS_RECTANGLE_HPP
#define PTK_WIDGETS_RECTANGLE_HPP

// Local Headers
#include "ptk/core/shape.hpp"

namespace pTK
{
    /** Rectangle class implementation.
     
     Derived from Shape, this class if for creating a
     simple rectangle for drawing.
     */
    class Rectangle : public Shape
    {
    public:
        /** Constructs Rectangle with default values.
         
         @return    default initialized Rectangle
         */
        Rectangle();
        virtual ~Rectangle() = default;
        
        /** Draw function.
         Function is called when it is time to draw.
         
         Derived from Drawable.
         */
        void draw(SkCanvas*) const override;
        
        /** Function for setting the corner radius.
         
         @param radius  corner radius
         */
        void set_corner_radius(float radius);
        
        /** Function for retrieving the corner radius.
         
         @return    corner radius
         */
        float get_corner_radius() const;
        
    private:
        float m_corner_radius;
    };
    
    // Comparison operators.
    bool operator==(const Rectangle& lhs, const Rectangle& rhs);
    bool operator!=(const Rectangle& lhs, const Rectangle& rhs);
}

#endif // PTK_WIDGETS_RECTANGLE_HPP
