//
//  platform/common/RasterContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

#ifndef PTK_PLATFORM_COMMON_RASTERCONTEXT_HPP
#define PTK_PLATFORM_COMMON_RASTERCONTEXT_HPP

// pTK Headers
#include "ptk/core/ContextBase.hpp"
#include "ptk/core/Exception.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "src/core/SkAutoMalloc.h"
PTK_DISABLE_WARN_END()

namespace pTK::Platform
{
    /** RasterContext class implementation.

        Context for a software based Rendering backend.
        All drawings will be done using the CPU.

        Be sure to override:
            - void swapBuffers();
            - void* onResize(const Size&);

        And, the constructor must call resize().
    */
    class PTK_API RasterContext : public ContextBase
    {
    public:
        /** Constructs RasterContext with default values.

            @return    default initialized RasterContext
        */
        RasterContext(SkColorType colorType, const Size& size);

        /** RasterContext destructor.

        */
        virtual ~RasterContext();

        /** Function for resizing the context.

            @param size   New width and height of canvas
        */
        void resize(const Size& size) override;

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        [[nodiscard]] sk_sp<SkSurface> surface() const override { return m_surface; }

    private:
        // Called on resize, return the pointer to pixel storage, on failure return nullptr.
        virtual void* onResize(const Size& UNUSED(size)) = 0;

    private:
        sk_sp<SkSurface> m_surface;
        SkColorType m_colorType;
    };
} // namespace pTK

#endif // PTK_PLATFORM_COMMON_RASTERCONTEXT_HPP
