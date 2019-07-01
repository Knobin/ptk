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
        m_size = size;
        redraw();
    }
    
    void Widget::setMinSizeHint(const Size& size)
    {
        m_minSize = size;
        notifyParent();
    }
    
    void Widget::setMaxSizeHint(const Size& size)
    {
        m_maxSize = size;
        notifyParent();
    }
    
    void Widget::setPosHint(const Position& pos)
    {
        m_pos = pos;
        redraw();
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
    
    bool Widget::redraw() const
    {
        if (m_parent != nullptr)
            return m_parent->redrawChild(this);
        
        return false;
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
