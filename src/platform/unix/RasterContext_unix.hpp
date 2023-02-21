//
//  platform/unix/RasterContext_unix.hpp
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
    /** RasterContext_unix class implementation.

        Raster Context for the Windows backend.
    */
    class PTK_API RasterContext_unix : public RasterContext
    {
    public:
        /** Deleted constructor for RasterContext_unix

        */
        RasterContext_unix() = delete;

        /** Constructs RasterContext_unix with default values.

            @param window   xwindow pointer
            @param size     context size
            @param info     xvisualinfo pointer
            @return         default initialized RasterContext_unix
        */
        RasterContext_unix(::Window window, const Size& size, XVisualInfo info);

        /** Destructor for RasterContext_unix.

        */
        ~RasterContext_unix();

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
