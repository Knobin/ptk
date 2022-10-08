//
//  core/Shape.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// pTK Headers
#include "ptk/core/Shape.hpp"

namespace pTK
{
    Shape::Shape()
        : Widget(),
            m_color{0xf5f5f5ff}, m_outlineColor{0xf5f5f5ff}, m_outlineThickness{0}
    {}
    
    const Color& Shape::getColor() const
    {
        return m_color;
    }
    
    void Shape::setColor(const Color& color)
    {
        m_color = color;
        draw();
    }
    
    const Color& Shape::getOutlineColor() const
    {
        return m_outlineColor;
    }
    
    void Shape::setOutlineColor(const Color& outline_color)
    {
        m_outlineColor = outline_color;
        draw();
    }
    
    float Shape::getOutlineThickness() const
    {
        return m_outlineThickness;
    }
    
    void Shape::setOutlineThickness(float outlineThickness)
    {
        m_outlineThickness = outlineThickness;
        draw();
    }
    
    // Comparison operators.
    bool operator==(const Shape& lhs, const Shape& rhs)
    {
        return ((lhs.getColor() == rhs.getColor()) &&
                (lhs.getOutlineColor() == rhs.getOutlineColor()) &&
                (lhs.getOutlineThickness() == rhs.getOutlineThickness()));
    }
    
    bool operator!=(const Shape& lhs, const Shape& rhs)
    {
        return !(lhs == rhs);
    }
}
