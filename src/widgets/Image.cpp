//
//  widgets/Image.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-25.
//

// Local Headers
#include "../Log.hpp"

// pTK Headers
#include "ptk/widgets/Image.hpp"
#include "ptk/core/ContextBase.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkData.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    Image::Image()
        : Widget(),
          m_path{},
          m_image{nullptr},
          m_scale{1.0f, 1.0f}
    {}

    Image::Image(const std::string& path)
        : Widget(),
          m_path{path},
          m_image{nullptr},
          m_scale{1.0f, 1.0f}
    {
        loadFromFile(path);
    }

    bool Image::loadFromFile(const std::string& path)
    {
        sk_sp<SkData> data{SkData::MakeFromFileName(path.c_str())};
        if (data)
        {
            PTK_INFO("Loaded \"{}\" successfully.", path);
            m_image = SkImage::MakeFromEncoded(data);
            if (m_image)
            {
                PTK_INFO("Created image from \"{}\" successfully.", path);
                m_path = path;
                const float w = static_cast<float>(m_image->width()) * m_scale.x;
                const float h = static_cast<float>(m_image->height()) * m_scale.y;
                setSize(Size(static_cast<Size::value_type>(w), static_cast<Size::value_type>(h)));
                return true;
            }
        }
        PTK_ERROR("Error loading File \"{}\"!", path);
        return false;
    }

    bool Image::isLoaded() const
    {
        if (m_image)
            return true;

        return false;
    }

    void Image::onDraw(Canvas* canvas)
    {
        if (m_image)
            canvas->drawImage(getPosition(), getSize(), m_image.get());
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

        if (m_image)
        {
            const float w = static_cast<float>(m_image->width()) * m_scale.x;
            const float h = static_cast<float>(m_image->height()) * m_scale.y;
            setSize(Size{static_cast<Size::value_type>(w), static_cast<Size::value_type>(h)});
        }
    }
} // namespace pTK
