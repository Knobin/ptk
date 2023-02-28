//
//  src/platform/mac/RasterContextMac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PLATFORM_MAC_RASTERCONTEXT_HPP
#define PTK_PLATFORM_MAC_RASTERCONTEXT_HPP

// pTK Headers
#include "ptk/platform/RasterContext.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkImageInfo.h"
PTK_DISABLE_WARN_END()

// macOS Headers
#include <Cocoa/Cocoa.h>

namespace pTK::Platform
{
    /** RasterContextMac class implementation.

        Raster Context for the macOS backend.
    */
    class PTK_API RasterContextMac : public RasterContext
    {
    public:
        /** Deleted constructor for RasterContextMac

        */
        RasterContextMac() = delete;

        /** Constructs RasterContextMac with default values.

            @param window   valid pointer to NSWindow
            @param size     size of the context
            @return         default initialized RasterContextMac with window and size
        */
        RasterContextMac(NSWindow* window, const Size& size);

        /** Destructor for RasterContextMac.

        */
        virtual ~RasterContextMac();

        /** Function for resizing.

            @param size     new size
            @return         pointer to pixel storage, nullptr if error
        */
        void* onResize(const Size& size) override;

        /** Function for swapping the buffers.

        */
        void swapBuffers() override;

    private:
        NSWindow* m_window;
        CGContextRef m_gc;
        uint32_t* m_buffer{nullptr};
    };
} // namespace pTK::Platform

#endif // PTK_PLATFORM_MAC_RASTERCONTEXT_HPP
