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
        : m_minSize{pTK::Size::Min}, m_size{size}, m_maxSize{pTK::Size::Max},
            m_minLock{false}, m_maxLock{false}
    {
    }
    
    void Sizable::setMinSize(const Size& size)
    {
        bool changed = false;
        if ((size.height >= Size::Limits::Min) && (size.height <= m_size.height))
        {
            m_minSize.height = size.height;
            m_minLock = false;
            changed = true;
        }
        
        if ((size.width >= Size::Limits::Min) && (size.width <= m_size.width))
        {
            m_minSize.width = size.width;
            m_minLock = false;
            changed = true;
        }

        if (changed)
            onLimitChange(getMinSize(), getMaxSize());
    }
    
    Size Sizable::getMinSize() const
    {
        if (m_minLock)
            return m_size;
        
        return m_minSize;
    }
    
    void Sizable::setSize(const Size& size)
    {
        bool changed = false;
        if ((size.width >= m_minSize.width) && (size.width <= m_maxSize.width))
        {
            m_size.width = size.width;
            changed = true;
        }
            
        if ((size.height >= m_minSize.height) && (size.height <= m_maxSize.height))
        {
            m_size.height = size.height;
            changed = true;
        }
            
        if (changed)
            onResize(m_size);
    }
    
    void Sizable::setConstSize(const Size& size)
    {
        m_size = size;
        m_minLock = true;
        m_maxLock = true;
        onLimitChange(getMinSize(), getMaxSize());
        onResize(size);
    }
    
    const Size& Sizable::getSize() const
    {
        return m_size;
    }
    
    void Sizable::setMaxSize(const Size& size)
    {
        bool changed = false;
        if ((size.height >= m_size.height) && (size.height <= Size::Limits::Max))
        {
            m_maxSize.height = size.height;
            m_maxLock = false;
            changed = true;
        }
        
        if ((size.width >= m_size.width) && (size.width <= Size::Limits::Max))
        {
            m_maxSize.width = size.width;
            m_maxLock = false;
            changed = true;
        }

        if (changed)
            onLimitChange(getMinSize(), getMaxSize());
    }
    
    Size Sizable::getMaxSize() const
    {
        if (m_maxLock)
            return m_size;
        
        return m_maxSize;
    }

    void Sizable::setLimits(const Size& min, const Size& max)
    {
        bool changed = false;

        // Minimal size
        if ((min.height >= Size::Limits::Min) && (min.height <= m_size.height))
        {
            m_minSize.height = min.height;
            m_minLock = false;
            changed = true;
        }
        if ((min.width >= Size::Limits::Min) && (min.width <= m_size.width))
        {
            m_minSize.width = min.width;
            m_minLock = false;
            changed = true;
        }

        // Maximum size
        if ((max.height >= m_size.height) && (max.height <= Size::Limits::Max))
        {
            m_maxSize.height = max.height;
            m_maxLock = false;
            changed = true;
        }
        if ((max.width >= m_size.width) && (max.width <= Size::Limits::Max))
        {
            m_maxSize.width = max.width;
            m_maxLock = false;
            changed = true;
        }

        if (changed)
            onLimitChange(getMinSize(), getMaxSize());
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
