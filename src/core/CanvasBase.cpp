//
//  core/CanvasBase.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

// Local Headers
#include "ptk/core/CanvasBase.hpp"

namespace pTK
{
    CanvasBase::CanvasBase(const Size& size)
            : Singleton(),
                m_size{size}
    {

    }

    void CanvasBase::clear(const Color& color)
    {
        skCanvas()->clear(SkColorSetARGB(255, color.r, color.g, color.b));
    }

    const Size& CanvasBase::getSize() const
    {
        return m_size;
    }

    void CanvasBase::setSize(const Size& size)
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
