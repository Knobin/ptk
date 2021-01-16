//
//  core/Sizable.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-11.
//

// pTK Headers
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
        if (size == getMinSize())
            return;
        
        // Update minimal sizes.
        byte status{updateMinWidth(size.width)};
        status |= updateMinHeight(size.height);

        // Signal a size change.
        if (status & 1) // Check if first bit is set.
            onSizeChange(getSize());

        // Signal a limit change.
        if (status & 2) // Check if second bit is set.
        {
            m_minLock = false;
            onLimitChange(getMinSize(), getMaxSize());
        }
    }

    Size Sizable::getMinSize() const
    {
        // If SetConstSize has been called, return const size.
        if (m_minLock)
            return m_size;

        return m_minSize;
    }

    void Sizable::setSize(const Size& size)
    {
        if (size == getSize())
            return;
        
        bool changed = false;

        // Height
        if ((size.height >= m_minSize.height) && (size.height <= m_maxSize.height))
        {
            // Update height.
            m_size.height = size.height;
            changed = true;
        }

        // Width
        if ((size.width >= m_minSize.width) && (size.width <= m_maxSize.width))
        {
            // Update width.
            m_size.width = size.width;
            changed = true;
        }

        // Signal a size change.
        if (changed)
        {
            m_minLock = false;
            m_maxLock = false;
            onSizeChange(m_size);
        }
    }

    void Sizable::setConstSize(const Size& size)
    {
        // Update size and set locks.
        m_size = size;
        m_minLock = true;
        m_maxLock = true;

        // Signal events.
        onSizeChange(size);
        onLimitChange(getMinSize(), getMaxSize());
    }

    const Size& Sizable::getSize() const
    {
        return m_size;
    }

    void Sizable::setMaxSize(const Size& size)
    {
        if (size == getMaxSize())
            return;
        
        // Update maximum sizes.
        byte status{updateMaxWidth(size.width)};
        status |= updateMaxHeight(size.height);

        // Signal a size change.
        if (status & 1) // Check if first bit is set.
            onSizeChange(getSize());

        // Signal a limit change.
        if (status & 2) // Check if second bit is set.
        {
            m_maxLock = false;
            onLimitChange(getMinSize(), getMaxSize());
        }
    }

    Size Sizable::getMaxSize() const
    {
        // If SetConstSize has been called, return const size.
        if (m_maxLock)
            return m_size;

        return m_maxSize;
    }

    void Sizable::setLimits(const Size& min, const Size& max)
    {
        // Update minimal sizes.
        byte s1{updateMinWidth(min.width)};
        s1 |= updateMinHeight(min.height);

        // Update maximum sizes.
        byte s2{updateMaxWidth(max.width)};
        s2 |= updateMaxHeight(max.height);

        // Signal a size change.
        if ((s1 & 1) || ((s2 & 1)))
            onSizeChange(getSize());

        // Check if locks are needed.
        if (s1 & 2)
            m_minLock = false;
        if (s2 & 2)
            m_maxLock = false;

        // Signal a limit change.
        if ((s1 & 2) || ((s2 & 2)))
            onLimitChange(getMinSize(), getMaxSize());
    }

    bool Sizable::isConstSize() const
    {
        return m_minLock && m_maxLock;
    }

    byte Sizable::updateMinWidth(const Size::value_type width)
    {
        byte status{0};
        const Size::value_type maxWidth{getMaxSize().width};

        if ((width >= Size::Limits::Min) && (width <= maxWidth))
        {
            // If new minimal width is larger than current size. Set width to minimal width.
            if (width > m_size.width)
            {
                m_size.width = width;
                status = 1;
            }

            // Update minimal width.
            m_minSize.width = width;
            status |= 2;
        }

        return status;
    }

    byte Sizable::updateMinHeight(const Size::value_type height)
    {
        byte status{0};
        const Size::value_type maxHeight{getMaxSize().height};

        if ((height >= Size::Limits::Min) && (height <= maxHeight))
        {
            // If new minimal height is larger than current size. Set height to minimal height.
            if (height > m_size.height)
            {
                m_size.height = height;
                status = 1;
            }

            // Update minimal height.
            m_minSize.height = height;
            status |= 2;
        }

        return status;
    }

    byte Sizable::updateMaxWidth(const Size::value_type width)
    {
        byte status{0};
        const Size::value_type minWidth{getMinSize().width};

        if ((width >= minWidth) && (width <= Size::Limits::Max))
        {
            // If new maximum width is smaller than current size. Set width to maximum width.
            if (width < m_size.width)
            {
                m_size.width = width;
                status = 1;
            }

            // Update maximum width.
            m_maxSize.width = width;
            status |= 2;
        }

        return status;
    }

    byte Sizable::updateMaxHeight(const Size::value_type height)
    {
        byte status{0};
        const Size::value_type minHeight{getMinSize().height};

        if ((height >= minHeight) && (height <= Size::Limits::Max))
        {
            // If new maximum height is smaller than current size. Set height to maximum height.
            if (height < m_size.height)
            {
                m_size.height = height;
                status = 1;
            }

            // Update maximum height.
            m_maxSize.height = height;
            status |= 2;
        }

        return status;
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
