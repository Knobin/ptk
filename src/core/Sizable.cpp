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
        : m_minSize{pTK::Size::Min}, m_size{0, 0}, m_maxSize{pTK::Size::Max},
            m_minLock{false}, m_maxLock{false}
    {
    }
    
    Sizable::Sizable(const Size& size)
        : m_minSize{}, m_size{size}, m_maxSize{}
    {
    }
    
    void Sizable::setMinSize(const Size& size)
    {
        if ((size.height >= Size::Limits::Min) && (size.height <= m_size.height))
        {
            m_minSize.height = size.height;
            m_minLock = false;
        }
        
        if ((size.width >= Size::Limits::Min) && (size.width <= m_size.width))
        {
            m_minSize.width = size.width;
            m_minLock = false;
        }
    }
    
    Size Sizable::getMinSize() const
    {
        if (m_minLock)
            return m_size;
        
        return m_minSize;
    }
    
    void Sizable::setSize(const Size& size)
    {
        if ((size.width >= m_minSize.width) && (size.width <= m_maxSize.width))
            m_size.width = size.width;
        
        if ((size.height >= m_minSize.height) && (size.height <= m_maxSize.height))
            m_size.height = size.height;
    }
    
    void Sizable::setConstSize(const Size& size)
    {
        m_size = size;
        m_minLock = true;
        m_maxLock = true;
    }
    
    const Size& Sizable::getSize() const
    {
        return m_size;
    }
    
    void Sizable::setMaxSize(const Size& size)
    {
        if ((size.height >= m_size.height) && (size.height <= Size::Limits::Max))
        {
            m_maxSize.height = size.height;
            m_maxLock = false;
        }
        
        if ((size.width >= m_size.width) && (size.width <= Size::Limits::Max))
        {
            m_maxSize.width = size.width;
            m_maxLock = false;
        }
    }
    
    Size Sizable::getMaxSize() const
    {
        if (m_maxLock)
            return m_size;
        
        return m_maxSize;
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
