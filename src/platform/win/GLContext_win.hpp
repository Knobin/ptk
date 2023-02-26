//
//  platform/win/GLContext_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-20.
//

#ifndef PTK_PlATFORM_WIN_GLCONTEXT_HPP
#define PTK_PlATFORM_WIN_GLCONTEXT_HPP

// pTK Headers
#include "ptk/core/ContextBase.hpp"

// Windows Headers
#define NOMINMAX
#include <windows.h>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkColorSpace.h"
#include "include/gpu/GrDirectContext.h"
PTK_DISABLE_WARN_END()

namespace pTK::Platform
{
    /** GLContext_win class implementation.

        Context for a hardware based OpenGL Windows backend.
        All drawings will be done using the GPU.
    */
    class PTK_API GLContext_win : public ContextBase
    {
    public:
        /** Constructs GLContext_win with hwnd and size.

            @param hwnd     win32 window handle
            @param size     size of the context
            @return         initialized GLContext_win
        */
        GLContext_win(HWND hwnd, const Size& size);

        /** Win32RasterCanvas for GLCanvas.

        */
        virtual ~GLContext_win();

        /** Function for resizing the context.

            @param size     new size
        */
        void resize(const Size& size) override;

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        [[nodiscard]] sk_sp<SkSurface> surface() const override;

        /** Function for swapping the buffers

        */
        void swapBuffers() override;

    private:
        void createContext(const Size& size);
        void destroyContext();

    private:
        HWND m_hwnd;
        HGLRC m_hglrc;
        sk_sp<SkSurface> m_surface;
        sk_sp<const GrGLInterface> m_backendContext;
        sk_sp<GrDirectContext> m_context;
        GrContextOptions m_GrContextOptions;
        SkSurfaceProps m_props;
        int m_stencilBits{8};
        int m_sampleCount{1};
    };
} // namespace pTK::Platform

#endif // PTK_PlATFORM_WIN_GLCONTEXT_HPP
