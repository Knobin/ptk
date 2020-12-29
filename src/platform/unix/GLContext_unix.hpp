//
//  platform/unix/GLContext_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-12-26.
//

#ifndef PTK_PlATFORM_UNIX_GLCONTEXT_HPP
#define PTK_PlATFORM_UNIX_GLCONTEXT_HPP

// Local Headers
#include "ptk/core/platform/ContextBase.hpp"
#include "x11.hpp"

// OpenGL Headers
#include <GL/gl.h>
#include <GL/glx.h>

// Skia Headers
#include "include/gpu/GrContext.h"

namespace pTK
{
    /** GLContext_unix class implementation.

        Context for a hardware based OpenGL Unix backend.
        All drawings will be done using the GPU.
    */
    class GLContext_unix : public ContextBase
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
        sk_sp<GrContext> m_context;
        GrGLFramebufferInfo m_info;
        SkColorType m_colorType;
        SkSurfaceProps m_props;
    };
}

#endif // PTK_PlATFORM_UNIX_GLCONTEXT_HPP
