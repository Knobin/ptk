//
//  platform/unix/RasterContextUnix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-17.
//

// Local Headers
#include "RasterContextUnix.hpp"
#include "ApplicationHandleUnix.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkSurface.h"
PTK_DISABLE_WARN_END()

namespace pTK::Platform
{
    using App = ApplicationHandleUnix;

    RasterContextUnix::RasterContextUnix(::Window window, const Size& size, XVisualInfo info)
        : RasterContext(kBGRA_8888_SkColorType, size),
          m_window{window},
          m_info{info},
          m_image{nullptr}
    {
        m_gc = XCreateGC(App::Display(), m_window, 0, nullptr);
        resize(size);
    }

    RasterContextUnix::~RasterContextUnix()
    {
        delete[] static_cast<uint32_t*>(m_buffer);
    }

    void* RasterContextUnix::onResize(const Size& size)
    {
        if (m_image)
        {
            // XDestroyImage frees both the image structure and the data pointer.
            m_image->data = nullptr;
            XDestroyImage(m_image);
            m_image = nullptr;
            delete[] static_cast<uint32_t*>(m_buffer);
        }

        const auto width{static_cast<unsigned int>(size.width)};
        const auto height{static_cast<unsigned int>(size.height)};

        m_buffer = new (std::nothrow) uint32_t[width * height];
        if (m_buffer == nullptr)
            return nullptr;

        // const auto width = static_cast<unsigned int>(size.width);
        // const auto height = static_cast<unsigned int>(size.height);
        m_image = XCreateImage(App::Display(), m_info.visual, 24, ZPixmap, 0, reinterpret_cast<char*>(m_buffer), width,
                               height, 32, 0);

        return m_buffer;
    }

    void RasterContextUnix::swapBuffers()
    {
        const auto size{getSize()};
        const auto width = static_cast<unsigned int>(size.width);
        const auto height = static_cast<unsigned int>(size.height);

        XPutImage(App::Display(), m_window, m_gc, m_image, 0, 0, 0, 0, width, height);
    }
} // namespace pTK::Platform
