//
//  core/Widget.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// Local Headers
#include "ptk/core/Widget.hpp"
#include "ptk/Log.hpp"

namespace pTK
{
    Widget::Widget()
        : Drawable(), EventHandling(), Sizable(), m_parent{nullptr}, m_name{},
            m_margin{}
    {
        setMinSize(Size(pTK::Auto, pTK::Auto));
        setMaxSize(Size(pTK::Auto, pTK::Auto));
    }
    
    void Widget::setParent(Widget* parent)
    {
        m_parent = parent;
    }
    
    Widget* Widget::getParent() const
    {
        return m_parent;
    }
    
    void Widget::setSize(const Size& size)
    {
        Sizable::setSize(size);
        draw();
    }
    
    void Widget::setPosHint(const Point& pos)
    {
        m_pos = pos;
        draw();
    }
    
    const Point& Widget::getPosition() const
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
    
    void Widget::setMargin(const Margin& margin)
    {
        if ((margin.top > -3)
            && (margin.bottom > -3)
            && (margin.left > -3)
            && (margin.right > -3))
        {
            m_margin = margin;
            draw();
        }
    }
    
    void Widget::setMarginTop(int32 topMargin)
    {
        if (topMargin > -3)
        {
            m_margin.top = topMargin;
            draw();
        }
    }
    
    void Widget::setMarginBottom(int32 bottomMargin)
    {
        if (bottomMargin > -3)
        {
            m_margin.bottom = bottomMargin;
            draw();
        }
    }
    
    void Widget::setMarginLeft(int32 leftMargin)
    {
        if (leftMargin > -3)
        {
            m_margin.left = leftMargin;
            draw();
        }
    }
    
    void Widget::setMarginRight(int32 rightMargin)
    {
        if (rightMargin > -3)
        {
            m_margin.right = rightMargin;
            draw();
        }
    }
    
    void Widget::setMarginTopBottom(int32 topMargin, int32 bottomMargin)
    {
        if ((topMargin > -3)
            && (bottomMargin > -3))
        {
            m_margin.top = topMargin;
            m_margin.bottom = bottomMargin;
            draw();
        }
    }
    
    void Widget::setMarginLeftRight(int32 leftMargin, int32 rightMargin)
    {
        if ((leftMargin > -3)
            && (rightMargin > -3))
        {
            m_margin.left = leftMargin;
            m_margin.right = rightMargin;
            draw();
        }
    }
    
    const Margin& Widget::getMargin() const
    {
        return m_margin;
    }
    
    int32 Widget::getMarginTop() const
    {
        return m_margin.top;
    }
    
    int32 Widget::getMarginBottom() const
    {
        return m_margin.bottom;
    }
    
    int32 Widget::getMarginLeft() const
    {
        return m_margin.left;
    }
    
    int32 Widget::getMarginRight() const
    {
        return m_margin.right;
    }
    
    bool Widget::drawChild(Widget*)
    {
        return true;
    }
    
    bool Widget::draw()
    {
        if (m_parent != nullptr)
            return m_parent->drawChild(this);
        
        return false;
    }
    
    ///////////////////////////////////////////////////////////////////////////////
    
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
