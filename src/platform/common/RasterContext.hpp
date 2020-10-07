//
//  platform/common/RasterContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_PlATFORM_COMMON_RASTERCONTEXT_HPP
#define PTK_PlATFORM_COMMON_RASTERCONTEXT_HPP

// Local Headers
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
        RasterContext(const Size& size, Policy policy)
            : ContextBase(size), m_policy{policy}
        {
            resize(size);
        }

        /** RasterContext destructor.

        */
        virtual ~RasterContext() = default;

        /** Function for resizing the context.

            @param size   New width and height of canvas
        */
        void resize(const Size& size) override
        {
            m_surface.reset();

            if (!m_policy.resize(size))
                throw ContextError("Policy failed to resize in Raster Context");

            m_surfaceMemory.reset(m_policy.size);
            void *pixels{static_cast<void*>(m_policy.pixels)};

            const int w{static_cast<int>(size.width)};
            const int h{static_cast<int>(size.height)};
            SkImageInfo info{SkImageInfo::Make(w, h, m_policy.colorType, kPremul_SkAlphaType,nullptr)};
            m_surface = SkSurface::MakeRasterDirect(info, pixels, sizeof(uint32_t) * static_cast<unsigned long>(w));
            if (!m_surface)
                throw ContextError("Failed to create Raster Context");

            PTK_INFO("Created Rastercontext: {}x{} {}", size.width, size.height, m_policy.size);
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
        SkAutoMalloc  m_surfaceMemory;
        sk_sp<SkSurface> m_surface;
        Policy m_policy;
    };
}

#endif // PTK_PlATFORM_COMMON_RASTERCONTEXT_HPP
