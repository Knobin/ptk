//
//  src/platform/mac/RasterContext_mac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PLATFORM_MAC_RASTERCONTEXT_HPP
#define PTK_PLATFORM_MAC_RASTERCONTEXT_HPP

// pTK Headers
#include "ptk/platform/common/RasterContext.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkImageInfo.h"
PTK_DISABLE_WARN_END()

// macOS Headers
#include <Cocoa/Cocoa.h>

namespace pTK::Platform
{
    /** RasterContext_mac class implementation.

        Raster Context for the macOS backend.
    */
    class PTK_API RasterContext_mac : public RasterContext
    {
    public:
        /** Deleted constructor for RasterContext_mac

        */
        RasterContext_mac() = delete;

        /** Constructs RasterContext_mac with default values.

            @param window   NSWindow pointer
            @param size     context size
            @return         default initialized RasterContext_mac
        */
        RasterContext_mac(NSWindow* window, const Size& size);

        /** Destructor for RasterContext_mac.

        */
        virtual ~RasterContext_mac();

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
