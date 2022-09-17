//
//  platform/unix/GLContext_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-12-26.
//

#ifndef PTK_PlATFORM_UNIX_GLCONTEXT_HPP
#define PTK_PlATFORM_UNIX_GLCONTEXT_HPP

// Local Headers
#include "ptk/platform/unix/x11.hpp"

// pTK Headers
#include "ptk/platform/base/ContextBase.hpp"

// OpenGL Headers
extern "C" {
    #define Status int
    #include <GL/gl.h>
    #include <GL/glx.h>
    #undef Status
}

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
//#include "include/gpu/GrContext.h"
#include "include/gpu/GrDirectContext.h"
#include "include/gpu/gl/GrGLInterface.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    /** GLContext_unix class implementation.

        Context for a hardware based OpenGL Unix backend.
        All drawings will be done using the GPU.
    */
    class PTK_API GLContext_unix : public ContextBase
    {
    public:
        /** Constructs GLContext_unix with default values.

            @return    default initialized GLContext_unix
        */
        GLContext_unix(::Window *window, const Size& size);

        /** Win32RasterCanvas for GLCanvas.

        */
        virtual ~GLContext_unix();

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
        ::Window *m_window;
        XVisualInfo* m_visual;
        GLXContext m_GLContext;
        sk_sp<SkSurface> m_surface;
        sk_sp<GrDirectContext> m_context;
        sk_sp<const GrGLInterface> m_backendContext;
        GrContextOptions m_GrContextOptions;
        SkSurfaceProps m_props;
    };
}

#endif // PTK_PlATFORM_UNIX_GLCONTEXT_HPP
