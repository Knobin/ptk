//
//  core/Widget.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// Local Headers
#include "ptk/core/Widget.hpp"

namespace pTK
{
    Widget::Widget()
        : Drawable(), EventHandling(), m_size{0, 0}, m_position{0 ,0}
    {
    }
    
    const Vec2<float>& Widget::getSize() const
    {
        return m_size;
    }
    
    void Widget::setSize(float width, float height)
    {
        m_size = {width, height};
    }
    
    void Widget::setSize(const Vec2<float>& size)
    {
        m_size = size;
    }
    
    const Vec2<float>& Widget::getPosition() const
    {
        return m_position;
    }
    
    void Widget::setPosition(float x, float y)
    {
        m_position = {x, y};
    }
    
    void Widget::setPosition(const Vec2<float>& position)
    {
        m_position = position;
    }
    
    void Widget::move(float offset_x, float offset_y)
    {
        m_position.x += offset_x;
        m_position.y += offset_y;
    }
    
    void Widget::move(const Vec2<float>& offset)
    {
        m_position += offset;
    }
    
    // Comparison operators.
    bool operator==(const Widget& lhs, const Widget& rhs)
    {
        return ((lhs.getPosition() == rhs.getPosition()) && (lhs.getSize() == rhs.getSize()));
    }
    
    bool operator!=(const Widget& lhs, const Widget& rhs)
    {
        return !(lhs == rhs);
    }
}
