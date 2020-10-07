//
//  platform/win/WinRasterContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-20.
//

#ifndef PTK_PlATFORM_WIN_WINGLCONTEXT_HPP
#define PTK_PlATFORM_WIN_WINGLCONTEXT_HPP

// Local Headers
#include "ptk/core/platform/ContextBase.hpp"

// Windows Headers
#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

// Skia Headers
#include "include/gpu/GrContext.h"

namespace pTK
{
    /** GLContext_win class implementation.

        Context for a hardware based OpenGL Windows backend.
        All drawings will be done using the GPU.
    */
    class GLContext_win : public ContextBase
    {
    public:
        /** Constructs GLContext_win with default values.

            @return    default initialized GLContext_win
        */
        GLContext_win(HWND handle, const Size& size);

        /** Win32RasterCanvas for GLCanvas.

        */
        virtual ~GLContext_win();

        /** Function for resizing the context.

            @param width   New width of canvas
            @param width   New height of canvas
        */
        void resize(const Size& size) override;

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        sk_sp<SkSurface> surface() const override;

        /** Function for swapping the buffers

        */
        void swapBuffers() override;

    private:
        HWND m_hwnd;
        HGLRC m_hglrc;
        sk_sp<SkSurface> m_surface;
        sk_sp<GrContext> m_context;
        GrGLFramebufferInfo m_info;
        SkColorType m_colorType;
        SkSurfaceProps m_props;
    };
}

#endif // PTK_PlATFORM_WIN_WINGLCONTEXT_HPP
