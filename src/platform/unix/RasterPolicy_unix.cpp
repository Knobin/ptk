//
//  platform/unix/RasterPolicy_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-17.
//

// Local Headers
#include "RasterPolicy_unix.hpp"
#include "ptk/Log.hpp"
#include "Application_unix.hpp"

// Skia Headers
#include "include/core/SkSurface.h"

using App = pTK::Application_unix;

namespace pTK
{
    RasterPolicy_unix::RasterPolicy_unix(::Window *window, XVisualInfo info)
        : m_window{window}, m_info{info}, m_image{nullptr}
    {
        m_gc = XCreateGC(App::Display(), *m_window, 0, nullptr);
    }

    RasterPolicy_unix::~RasterPolicy_unix()
    {
        delete [] static_cast<uint32_t*>(pixels);;
    }

    bool RasterPolicy_unix::resize(const Size& nSize)
    {
        if (m_image)
        {
            // XDestroyImage frees both the image structure and the data pointer.
            m_image->data = nullptr;
            XDestroyImage(m_image);
            m_image = nullptr;
            delete [] static_cast<uint32_t*>(pixels);
        }
        
        const std::size_t width{static_cast<std::size_t>(nSize.width)};
        const std::size_t height{static_cast<std::size_t>(nSize.height)};

        uint32_t *buffer = new (std::nothrow) uint32_t[width * height];
        if (!buffer)
            return false;

        pixels = static_cast<void*>(buffer);
        size = sizeof(uint32_t) * width * height;
        
        wSize = nSize;
        m_image = XCreateImage(App::Display(), m_info.visual, 24, ZPixmap, 0, static_cast<char*>(pixels), 
                                static_cast<unsigned int>(nSize.width), static_cast<unsigned int>(nSize.height), 32, 0);
        
        return true;
    }

    void RasterPolicy_unix::swapBuffers() const
    {
        XPutImage(App::Display(), *m_window, m_gc, m_image, 0, 0, 0, 0, 
                static_cast<unsigned int>(wSize.width), static_cast<unsigned int>(wSize.height));
    }
}
