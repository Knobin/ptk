//
//  widgets/Image.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-25.
//

// Local Headers
#include "../include/Log.hpp"

// pTK Headers
#include "ptk/widgets/Image.hpp"

namespace pTK
{
    Image::Image(std::shared_ptr<Texture> texture)
        : Widget(),
          m_texture{std::move(texture)}
    {
        applyScale(m_scale.x, m_scale.y);
    }

    Image::Image(const ImmutableBuffer& buffer)
        : Widget(),
          m_texture{std::make_shared<Texture>(buffer)}
    {
        applyScale(m_scale.x, m_scale.y);
    }

    Image::Image(const Pixmap& pixmap)
        : Widget(),
          m_texture{std::make_shared<Texture>(pixmap)}
    {
        applyScale(m_scale.x, m_scale.y);
    }

    bool Image::isValid() const
    {
        return (m_texture) ? m_texture->isValid() : false;
    }

    void Image::onDraw(Canvas* canvas)
    {
        if (m_texture)
            canvas->drawImage(getPosition(), getSize(), m_texture->skImage());
    }

    const Vec2f& Image::getScale() const
    {
        return m_scale;
    }

    void Image::setScale(float x, float y)
    {
        applyScale(x, y);
    }

    void Image::setScale(const Vec2f& scale)
    {
        applyScale(scale.x, scale.y);
    }

    void Image::applyScale(float x, float y)
    {
        if (x > 0.0f)
            m_scale.x = x;
        if (y > 0.0f)
            m_scale.y = y;

        if (m_texture)
            setSize(Size::MakeScaled(m_texture->width(), m_texture->height(), m_scale.x, m_scale.y, std::ceilf));
    }
} // namespace pTK
