//
//  core/Sizable.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-11.
//

// Local Headers
#include "ptk/core/Sizable.hpp"

namespace pTK
{
    Sizable::Sizable()
        : m_minSize{}, m_size{}, m_maxSize{}
    {
    }
    
    Sizable::Sizable(const Size& size)
        : m_minSize{}, m_size{size}, m_maxSize{}
    {
    }
    
    void Sizable::setMinSize(const Size& size)
    {
        m_minSize = size;
    }
    
    Size Sizable::getMinSize() const
    {
        Size size;
        
        if (m_minSize.height == pTK::Auto)
            size.height = m_size.height;
        else
            size.height = (m_minSize.height <= m_size.height) ? m_minSize.height : m_size.height;
        
        if (m_maxSize.width == pTK::Auto)
            size.width = m_size.width;
        else
            size.width = (m_minSize.width <= m_size.width) ? m_minSize.width : m_size.width;
        
        return size;
    }
    
    void Sizable::setSize(const Size& size)
    {
        m_size = size;
    }
    
    const Size& Sizable::getSize() const
    {
        return m_size;
    }
    
    void Sizable::setMaxSize(const Size& size)
    {
        m_maxSize = size;
    }
    
    Size Sizable::getMaxSize() const
    {
        Size size;
        
        if (m_maxSize.height == pTK::Auto)
            size.height = m_size.height;
        else
            size.height = (m_maxSize.height >= m_size.height) ? m_maxSize.height : m_size.height;
        
        if (m_maxSize.width == pTK::Auto)
            size.width = m_size.width;
        else
            size.width = (m_maxSize.width >= m_size.width) ? m_maxSize.width : m_size.width;
        
        return size;
    }
    
    // Comparison operators.
    bool operator==(const Sizable& lhs, const Sizable& rhs)
    {
        return ((lhs.getMinSize() == rhs.getMinSize()) &&
                (lhs.getSize() == rhs.getSize()) &&
                (lhs.getMaxSize() == rhs.getMaxSize()));
    }
    
    bool operator!=(const Sizable& lhs, const Sizable& rhs)
    {
        return !(lhs == rhs);
    }
}
