//
//  widgets/Rectangle.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_WIDGETS_RECTANGLE_HPP
#define PTK_WIDGETS_RECTANGLE_HPP

// pTK Headers
#include "ptk/core/Shape.hpp"

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
        void onDraw(SkCanvas* canvas) override;
        
        /** Function for setting the corner radius.
         
            @param radius  corner radius
        */
        void setCornerRadius(float radius);
        
        /** Function for retrieving the corner radius.
         
            @return    corner radius
        */
        float getCornerRadius() const;
        
    private:
        float m_cornerRadius;
    };
    
    // Comparison operators.
    bool operator==(const Rectangle& lhs, const Rectangle& rhs);
    bool operator!=(const Rectangle& lhs, const Rectangle& rhs);
}

#endif // PTK_WIDGETS_RECTANGLE_HPP
