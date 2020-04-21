//
//  platform/win32/Win32RasterContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-11.
//

#ifndef PTK_PlATFORM_WIN32_WIN32GLCONTEXT_HPP
#define PTK_PlATFORM_WIN32_WIN32GLCONTEXT_HPP

// Local Headers
#include "ptk/core/ContextBase.hpp"

// Windows Headers
#ifdef _WIN32
    #define NOMINMAX
    #include <windows.h>
#endif

// Skia Headers
#include "include/gpu/GrContext.h"

namespace pTK
{
    /** Win32GLContext class implementation.

        Context for a hardware based OpenGL Win32 backend.
        All drawings will be done using the GPU.
    */
    class Win32GLContext : public ContextBase
    {
    public:
        /** Constructs Win32GLContext with default values.

            @return    default initialized Win32GLContext
        */
        Win32GLContext(HWND handle, const Size& size);

        /** Win32RasterCanvas for GLCanvas.

        */
        virtual ~Win32GLContext();

        /** Function for resizing the context.

            @param width   New width of canvas
            @param width   New height of canvas
        */
        void resize(const Size& size) override;

        /** Function for retrieving the SkCanvas of the context.

            @return    SkCanvas property
        */
        SkCanvas* skCanvas() const override;

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        SkSurface* skSurface() const override;

        /** Function for swapping the buffers

        */
        void swapBuffers() override;

    private:
        HWND m_hwnd;
        HGLRC m_hglrc;
        sk_sp<SkSurface> m_surface;
        Ref<GrContext> m_context;
        GrGLFramebufferInfo m_info;
        SkColorType m_colorType;
        SkSurfaceProps m_props;
    };
}

#endif //PTK_PlATFORM_WIN32_WIN32GLCONTEXT_HPP
