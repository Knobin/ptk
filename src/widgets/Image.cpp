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
        : Widget(), m_path{}, m_image{nullptr}
    {
    }
    
    Image::Image(const std::string& path)
        : Widget(), m_path{path}, m_image{nullptr}
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
                setConstSize(Size(m_image->width(), m_image->height()));
                return true;
            }
        }
        PTK_INFO("Image \"{}\" not loaded!", path);
        return false;
    }
    
    void Image::onDraw(SkCanvas* canvas)
    {
        Point pos = getPosition();
        canvas->drawImage(m_image, pos.x, pos.y);
    }
}
