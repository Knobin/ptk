//
//  platform/win/GLContextWin.hpp
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
    /** GLContextWin class implementation.

        Context for a hardware based OpenGL Windows backend.
        All drawings will be done using the GPU.
    */
    class PTK_API GLContextWin : public ContextBase
    {
    public:
        /** Constructs GLContextWin with hwnd and size.

            @param hwnd     win32 window handle
            @param size     size of the context
            @return         initialized GLContextWin
        */
        GLContextWin(HWND hwnd, const Size& size);

        /** Win32RasterCanvas for GLCanvas.

        */
        virtual ~GLContextWin();

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
