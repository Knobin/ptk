//
//  widgets/Image.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-25.
//

// pTK Headers
#include "ptk/widgets/Image.hpp"

namespace pTK
{
    Image::Image()
        : Widget(),
            m_path{}, m_image{nullptr}, m_scale{1.0f, 1.0f}
    {}
    
    Image::Image(const std::string& path)
        : Widget(),
            m_path{path}, m_image{nullptr}, m_scale{1.0f, 1.0f}
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
                setSize(Size(static_cast<int>(m_image->width()*m_scale.x), static_cast<int>(m_image->height()*m_scale.y)));
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
    
    void Image::onDraw(SkCanvas* canvas)
    {
        if (m_image)
        {
            // Set Size and Position
            const SkPoint pos{convertToSkPoint(getPosition())};
            SkPoint size{convertToSkPoint(getSize())};
            size += pos; // skia needs the size to be pos+size.
            
            SkRect dst{};
            dst.set(pos, size);
            canvas->drawImageRect(m_image, dst, nullptr);
        } 
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

        m_scale = {x, y};

        if (m_image)
        {
            Size size;
            size.width  = static_cast<Size::value_type>(m_image->width()*x);
            size.height = static_cast<Size::value_type>(m_image->height()*y);
            setSize(size);
        }
    }
}
