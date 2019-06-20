//
//  core/shape.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// Local Headers
#include "ptk/core/shape.hpp"

namespace pTK
{
    Shape::Shape()
        : Widget(), m_color{0, 0, 0}, m_outline_color{0, 0, 0}, m_outline_thickness{0}
    {
    }
    
    const Color& Shape::get_color() const
    {
        return m_color;
    }
    
    void Shape::set_color(const Color& color)
    {
        m_color = color;
    }
    
    const Color& Shape::get_outline_color() const
    {
        return m_outline_color;
    }
    
    void Shape::set_outline_color(const Color& outline_color)
    {
        m_outline_color = outline_color;
    }
    
    float Shape::get_outline_thickness() const
    {
        return m_outline_thickness;
    }
    
    void Shape::set_outline_thickness(float outline_thickness)
    {
        m_outline_thickness = outline_thickness;
    }
    
    // Comparison operators.
    bool operator==(const Shape& lhs, const Shape& rhs)
    {
        return ((lhs.get_color() == rhs.get_color()) &&
                (lhs.get_outline_color() == rhs.get_outline_color()) &&
                (lhs.get_outline_thickness() == rhs.get_outline_thickness()));
    }
    
    bool operator!=(const Shape& lhs, const Shape& rhs)
    {
        return !(lhs == rhs);
    }
}
