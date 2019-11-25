//
//  widgets/Image.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-25.
//

// Local Headers
#include "ptk/widgets/Image.hpp"
#include "ptk/Log.hpp"

// Skia Headers
#include "include/core/SkData.h"

namespace pTK
{
    Image::Image()
        : Widget(), m_path{}, m_image{nullptr}, m_scale{1.0f, 1.0f}
    {
    }
    
    Image::Image(const std::string& path)
        : Widget(), m_path{path}, m_image{nullptr}, m_scale{1.0f, 1.0f}
    {
        loadFromFile(path);
    }
    
    bool Image::loadFromFile(const std::string& path)
    {
        auto data = SkData::MakeFromFileName(path.c_str());
        if (data)
        {
            PTK_INFO("Image \"{}\" loaded!", path);
            m_image = SkImage::MakeFromEncoded(data);
            if (m_image)
            {
                m_path = path;
                setConstSize(Size(static_cast<int>(m_image->width()*m_scale.x), static_cast<int>(m_image->height()*m_scale.y)));
                return true;
            }
        }
        PTK_INFO("Image \"{}\" not loaded!", path);
        return false;
    }
    
    void Image::onDraw(SkCanvas* canvas)
    {
        // Set Size and Position
        SkPoint pos{convertToSkPoint(getPosition())};
        SkPoint size{convertToSkPoint(getSize())};
        size += pos; // skia needs the size to be pos+size.
        
        SkRect dst;
        dst.set(pos, size);
        
        canvas->drawImageRect(m_image, dst, nullptr);
        
    }
    
    void Image::scale(float x, float y)
    {
        m_scale = {x, y};
        Size size;
        size.width = static_cast<int>(m_image->width()*x);
        size.height = static_cast<int>(m_image->height()*y);
        setConstSize(size);
    }
}
