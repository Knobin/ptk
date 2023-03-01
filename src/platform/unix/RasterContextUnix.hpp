//
//  platform/unix/RasterContextUnix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-17.
//

#ifndef PTK_PLATFORM_UNIX_RASTERCONTEXT_HPP
#define PTK_PLATFORM_UNIX_RASTERCONTEXT_HPP

// Local Headers
#include "x11.hpp"

// pTK Headers
#include "ptk/platform/RasterContext.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include <include/core/SkImageInfo.h>
PTK_DISABLE_WARN_END()

namespace pTK::Platform
{
    /** RasterContextUnix class implementation.

        Raster Context for the Unix backend.
    */
    class PTK_API RasterContextUnix : public RasterContext
    {
    public:
        /** Deleted constructor for RasterContextUnix

        */
        RasterContextUnix() = delete;

        /** Constructs RasterContextUnix with values.

            @param window   xlib window
            @param size     size of the context
            @param info     xvisualinfo pointer
            @return         initialized RasterContextUnix with values
        */
        RasterContextUnix(::Window window, const Size& size, XVisualInfo info);

        /** Destructor for RasterContextUnix.

        */
        ~RasterContextUnix();

        /** Function for resizing.

            @param size     new size
            @return         pointer to pixel storage, nullptr if error
        */
        void* onResize(const Size& size) override;

        /** Function to swap the buffers after drawing.

        */
        void swapBuffers() override;

    private:
        ::Window m_window;
        XVisualInfo m_info;
        XImage* m_image;
        GC m_gc;
        uint32_t* m_buffer{nullptr};
    };
} // namespace pTK::Platform

#endif // PTK_PLATFORM_UNIX_RASTERCONTEXT_HPP
