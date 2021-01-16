//
//  platform/common/RasterContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_PlATFORM_COMMON_RASTERCONTEXT_HPP
#define PTK_PlATFORM_COMMON_RASTERCONTEXT_HPP

// pTK Headers
#include "ptk/core/platform/ContextBase.hpp"
#include "ptk/core/Exception.hpp"

// Skia Headers
#include "src/core/SkAutoMalloc.h"

namespace pTK
{
    /** RasterContext class implementation.

        Context for a software based Rendering backend.
        All drawings will be done using the CPU.
    */
    template<typename Policy>
    class RasterContext : public ContextBase
    {
    public:
        /** Constructs RasterContext with default values.

            @return    default initialized RasterContext
        */
        RasterContext(const Size& size, const Policy& policy)
            : ContextBase(size), m_policy{policy}
        {
            PTK_INFO("Initialized RasterContext");
            resize(size);
        }

        /** RasterContext destructor.

        */
        virtual ~RasterContext()
        {
            PTK_INFO("Destroyed RasterContext");
        }

        /** Function for resizing the context.

            @param size   New width and height of canvas
        */
        void resize(const Size& size) override
        {
            m_surface.reset();

            if (!m_policy.resize(size))
                throw ContextError("Policy failed to resize in Raster Context");

            void *pixels{static_cast<void*>(m_policy.pixels)};

            const int w{static_cast<int>(size.width)};
            const int h{static_cast<int>(size.height)};
            SkImageInfo info{SkImageInfo::Make(w, h, m_policy.colorType, kPremul_SkAlphaType,nullptr)};
            m_surface = SkSurface::MakeRasterDirect(info, pixels, sizeof(uint32_t) * static_cast<decltype(sizeof(uint32_t))>(w));
            if (!m_surface)
                throw ContextError("Failed to create Raster Context");

            PTK_INFO("Sized RasterContext to {}x{}", size.width, size.height);
            setSize(size);
        }

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        sk_sp<SkSurface> surface() const override
        {
            return m_surface;
        }

        /** Function for swapping the buffers.

        */
        void swapBuffers() override
        {
            m_policy.swapBuffers();
        }

    private:
        sk_sp<SkSurface> m_surface;
        Policy m_policy;
    };
}

#endif // PTK_PlATFORM_COMMON_RASTERCONTEXT_HPP
