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
    
    void Sizable::setMinSize(const Size& size)
    {
        m_minSize = size;
    }
    
    const Size& Sizable::getMinSize() const
    {
        return m_minSize;
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
    
    const Size& Sizable::getMaxSize() const
    {
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
