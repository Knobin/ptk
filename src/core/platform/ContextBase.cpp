//
//  core/ContextBase.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

// pTK Headers
#include "ptk/core/platform/ContextBase.hpp"

namespace pTK
{
    ContextBase::ContextBase(const Size& size)
        : SingleObject(),
            m_size{size}
    {

    }

    const Size& ContextBase::getSize() const
    {
        return m_size;
    }

    void ContextBase::setSize(const Size& size)
    {
        m_size = size;
    }

    // Functions for converting utility classes to SkPoint for drawing.
    SkPoint convertToSkPoint(const Point& pos, const Vec2f& scale)
    {
        return SkPoint{static_cast<float>(pos.x)*scale.x, static_cast<float>(pos.y)*scale.y};
    }

    SkPoint convertToSkPoint(const Size& size, const Vec2f& scale)
    {
        return SkPoint{static_cast<float>(size.width)*scale.x, static_cast<float>(size.height)*scale.y};
    }
}
