//
//  platform/win/WinRasterContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-20.
//

#ifndef PTK_PlATFORM_WIN_WINGLCONTEXT_HPP
#define PTK_PlATFORM_WIN_WINGLCONTEXT_HPP

// pTK Headers
#include "ptk/platform/base/ContextBase.hpp"

// Windows Headers
#define NOMINMAX
#include <windows.h>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkColorSpace.h"
#include "include/gpu/GrDirectContext.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    /** GLContext_win class implementation.

        Context for a hardware based OpenGL Windows backend.
        All drawings will be done using the GPU.
    */
    class PTK_API GLContext_win : public ContextBase
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
}

#endif // PTK_PlATFORM_WIN_WINGLCONTEXT_HPP
