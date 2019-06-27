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
        : Drawable(), EventHandling(), m_parent{nullptr}, m_name{}
    {
    }
    
    void Widget::setParent(Container* parent)
    {
        m_parent = parent;
    }
    
    Container* Widget::getParent() const
    {
        return m_parent;
    }
    
    void Widget::setSizeHint(const Size& size)
    {
        Size backup = m_size;
        m_size = size;
        
        if (!notifyParent())
        {
            m_size = backup;
        }
    }
    
    void Widget::setMinSizeHint(const Size& size)
    {
        m_minSize = size;
    }
    
    void Widget::setMaxSizeHint(const Size& size)
    {
        m_maxSize = size;
    }
    
    void Widget::setPosHint(const Position& pos)
    {
        Position backup = m_pos;
        m_pos = pos;
        
        if (!notifyParent())
        {
            m_pos = backup;
        }
    }
    
    const Size& Widget::getSize() const
    {
        return m_size;
    }
    
    const Size& Widget::getMinSize() const
    {
        return m_minSize;
    }
    
    const Size& Widget::getMaxSize() const
    {
        return m_maxSize;
    }
    
    const Position& Widget::getPosition() const
    {
        return m_pos;
    }
    
    void Widget::setName(const std::string& name)
    {
        m_name = name;
    }
    
    const std::string& Widget::getName() const
    {
        return m_name;
    }
    
    bool Widget::notifyParent() const
    {
        if (m_parent != nullptr)
            return m_parent->verifyChild(this);
        
        return true;
    }
    
    // Comparison operators.
    bool operator==(const Widget& lhs, const Widget& rhs)
    {
        return ((lhs.getPosition() == rhs.getPosition()) &&
                (lhs.getSize() == rhs.getSize()) &&
                (lhs.getName() == rhs.getName()) &&
                (lhs.getParent() == rhs.getParent()));
    }
    
    bool operator!=(const Widget& lhs, const Widget& rhs)
    {
        return !(lhs == rhs);
    }
}
