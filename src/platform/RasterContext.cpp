//
//  platform/RasterContext.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

// Local Headers
#include "../Log.hpp"

// pTK Headers
#include "ptk/platform/RasterContext.hpp"

namespace pTK::Platform
{
    RasterContext::RasterContext(SkColorType colorType, const Size& size)
        : ContextBase(ContextBackendType::Raster, size),
          m_colorType{colorType}
    {
        PTK_INFO("Initialized RasterContext");
    }

    RasterContext::~RasterContext()
    {
        PTK_INFO("Destroyed RasterContext");
    }

    void RasterContext::resize(const Size& size)
    {
        m_surface.reset();

        void* pixels = static_cast<void*>(onResize(size));

        if (pixels == nullptr)
            throw ContextError("Failed to resize in Raster Context");

        const int w{static_cast<int>(size.width)};
        const int h{static_cast<int>(size.height)};
        SkImageInfo info{SkImageInfo::Make(w, h, m_colorType, kPremul_SkAlphaType, nullptr)};
        m_surface =
            SkSurface::MakeRasterDirect(info, pixels, sizeof(uint32_t) * static_cast<decltype(sizeof(uint32_t))>(w));
        if (!m_surface)
            throw ContextError("Failed to create Raster Context");

        PTK_INFO("Sized RasterContext to {}x{}", size.width, size.height);
        setSize(size);
    }

} // namespace pTK::Platform
