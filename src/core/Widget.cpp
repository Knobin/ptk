//
//  core/Widget.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// Local Headers
#include "ptk/core/Widget.hpp"
#include "ptk/core/Container.hpp"
#include "ptk/Log.hpp"

namespace pTK
{
    Widget::Widget()
        : Drawable(), EventHandling(), m_size{0, 0}, m_position{0 ,0}, m_parent{nullptr}
    {
    }
    
    const Vec2<float>& Widget::getSize() const
    {
        return m_size;
    }
    
    void Widget::setSize(float width, float height)
    {
        Vec2<float> backup = m_size;
        m_size = {width, height};
        
        if (m_parent != nullptr)
        {
            if (!m_parent->verifyChild(this))
            {
                PTK_WARN("[Widget] Parent did not accept size!\nFrom: {0:f}x{1:f} to {2:f}x{3:f}", backup.x, backup.y, m_position.x, m_position.y);
                m_size = backup;
            }
        }
    }
    
    void Widget::setSize(const Vec2<float>& size)
    {
        Vec2<float> backup = m_size;
        m_size = size;
        
        if (m_parent != nullptr)
        {
            if (!m_parent->verifyChild(this))
            {
                PTK_WARN("[Widget] Parent did not accept size!\nFrom: {0:f}x{1:f} to {2:f}x{3:f}", backup.x, backup.y, m_position.x, m_position.y);
                m_size = backup;
            }
        }
    }
    
    const Vec2<float>& Widget::getPosition() const
    {
        return m_position;
    }
    
    void Widget::setPosition(float x, float y)
    {
        Vec2<float> backup = m_position;
        m_position = {x, y};
        
        if (m_parent != nullptr)
        {
            if (!m_parent->verifyChild(this))
            {
                PTK_WARN("[Widget] Parent did not accept position!\nFrom: {0:f}x{1:f} to {2:f}x{3:f}", backup.x, backup.y, m_position.x, m_position.y);
                m_position = backup;
            }
        }
    }
    
    void Widget::setPosition(const Vec2<float>& position)
    {
        Vec2<float> backup = m_position;
        m_position = position;
        
        if (m_parent != nullptr)
        {
            if (!m_parent->verifyChild(this))
            {
                PTK_WARN("[Widget] Parent did not accept position!\nFrom: {0:f}x{1:f} to {2:f}x{3:f}", backup.x, backup.y, m_position.x, m_position.y);
                m_position = backup;
            }
        }
    }
    
    void Widget::move(float offset_x, float offset_y)
    {
        Vec2<float> backup = m_position;
        m_position.x += offset_x;
        m_position.y += offset_y;
        
        if (m_parent != nullptr)
        {
            if (!m_parent->verifyChild(this))
            {
                PTK_WARN("[Widget] Parent did not accept move!\nFrom: {0:f}x{1:f} to {2:f}x{3:f}", backup.x, backup.y, m_position.x, m_position.y);
                m_position = backup;
            }
        }
    }
    
    void Widget::move(const Vec2<float>& offset)
    {
        Vec2<float> backup = m_position;
        m_position += offset;
        
        if (m_parent != nullptr)
        {
            if (!m_parent->verifyChild(this))
            {
                PTK_WARN("[Widget] Parent did not accept move!\nFrom: {0:f}x{1:f} to {2:f}x{3:f}", backup.x, backup.y, m_position.x, m_position.y);
                m_position = backup;
            }
        }
    }
    
    void Widget::setParent(Container* parent)
    {
        m_parent = parent;
    }
    
    Container* Widget::getParent() const
    {
        return m_parent;
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
