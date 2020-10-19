//
//  platform/unix/RasterPolicy_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-17.
//

// Local Headers
#include "RasterPolicy_unix.hpp"
#include "ptk/Log.hpp"

// Skia Headers
#include "include/core/SkSurface.h"

namespace pTK
{
    RasterPolicy_unix::RasterPolicy_unix(Display *display, ::Window window, XVisualInfo info)
        : m_display{display}, m_window{window}, m_info{info}, m_image{nullptr}
    {
        m_gc = XCreateGC(m_display, m_window, 0, nullptr);
    }

    RasterPolicy_unix::~RasterPolicy_unix()
    {
        delete [] static_cast<uint32_t*>(pixels);;
    }

    bool RasterPolicy_unix::resize(const Size& nSize)
    {
        delete [] static_cast<uint32_t*>(pixels);
        if (m_image)
            XDestroyImage(m_image);

        const std::size_t width{static_cast<std::size_t>(nSize.width)};
        const std::size_t height{static_cast<std::size_t>(nSize.height)};

        uint32_t *buffer = new (std::nothrow) uint32_t[width * height];
        if (!buffer)
            return false;

        pixels = static_cast<void*>(buffer);
        size = sizeof(uint32_t) * width * height;
        
        wSize = nSize;
        m_image = XCreateImage(m_display, m_info.visual, 24, ZPixmap, 0, static_cast<char*>(pixels), 
                                static_cast<unsigned int>(nSize.width), static_cast<unsigned int>(nSize.height), 32, 0);
        
        return true;
    }

    void RasterPolicy_unix::swapBuffers() const
    {
        XPutImage(m_display, m_window, m_gc, m_image, 0, 0, 0, 0, 
                static_cast<unsigned int>(wSize.width), static_cast<unsigned int>(wSize.height));
    }
}
