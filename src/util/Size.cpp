//
//  util/Size.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

// Local Headers
#include "ptk/util/Size.hpp"

namespace pTK
{
    Size::Size()
        : m_width{0.0f}, m_height{0.0f}
    {
        
    }
    
    Size::Size(float width, float height)
        : m_width{width}, m_height{height}
    {
        
    }
    
    void Size::setSize(float width, float height)
    {
        m_width = width;
        m_height = height;
    }
    
    void Size::setSize(const Size& size)
    {
        m_width = size.getWidth();
        m_height = size.getHeight();
    }
    
    void Size::setWidth(float width)
    {
        m_width = width;
    }
    
    void Size::setHeight(float height)
    {
        m_height = height;
    }
    
    float Size::getHeight() const
    {
        return m_height;
    }
    
    float Size::getWidth() const
    {
        return m_width;
    }
    
    // Comparison operators.
    bool operator==(const Size& lhs, const Size& rhs)
    {
        return ((lhs.getWidth() == rhs.getWidth()) && (lhs.getHeight() == rhs.getHeight()));
    }
    
    bool operator!=(const Size& lhs, const Size& rhs)
    {
        return !(lhs == rhs);
    }
    
    // Binary arithmetic operators.
    Size operator+(const Size& lhs, const Size& rhs)
    {
        Size newSize;
        newSize.setWidth(lhs.getWidth() + rhs.getWidth());
        newSize.setHeight(lhs.getHeight() + rhs.getHeight());
        
        return newSize;
    }
    
    Size operator-(const Size& lhs, const Size& rhs)
    {
        Size newSize;
        newSize.setWidth(lhs.getWidth() - rhs.getWidth());
        newSize.setHeight(lhs.getHeight() - rhs.getHeight());
        
        return newSize;
    }
    
    Size operator*(const Size& lhs, const Size& rhs)
    {
        Size newSize;
        newSize.setWidth(lhs.getWidth() * rhs.getWidth());
        newSize.setHeight(lhs.getHeight() * rhs.getHeight());
        
        return newSize;
    }
    
    Size operator/(const Size& lhs, const Size& rhs)
    {
        Size newSize;
        newSize.setWidth(lhs.getWidth() / rhs.getWidth());
        newSize.setHeight(lhs.getHeight() / rhs.getHeight());
        
        return newSize;
    }
    
    Size& operator+=(Size& lhs, const Size& rhs)
    {
        return lhs = lhs + rhs;
    }
    
    Size& operator-=(Size& lhs, const Size& rhs)
    {
        return lhs = lhs - rhs;
    }
    
    Size& operator*=(Size& lhs, const Size& rhs)
    {
        return lhs = lhs * rhs;
    }
    
    Size& operator/=(Size& lhs, const Size& rhs)
    {
        return lhs = lhs / rhs;
    }
}
