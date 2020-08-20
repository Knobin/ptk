//
//  platform/glfw/GLContext_glfw.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

#ifndef PTK_PLATFORM_GLFW_GLCONTEXT_HPP
#define PTK_PLATFORM_GLFW_GLCONTEXT_HPP

// Local Headers
#include "ptk/core/platform/ContextBase.hpp"

// C++ Headers
#include <memory>

// OpenGL Headers
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl.h>
#else
	#include <GL/gl.h>
#endif

// Skia Headers
#include "include/gpu/GrContext.h"

namespace pTK
{
    /** GLContext_glfw class implementation.

        Canvas for setting up OpenGL rendering.
    */
    class GLContext_glfw : public ContextBase
    {
    public:
        /** Constructs GLContext_glfw with default values.

            @return    default initialized GLContext
        */
        GLContext_glfw(const Size& size);

        /** Deconstructor for GLContext.

        */
        virtual ~GLContext_glfw();

        /** Function for resizing the context.

            @param width   New width of context
            @param width   New height of context
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

    private:
        // Skia
        sk_sp<SkSurface> m_surface;
        Ref<GrContext> m_context;
        GrGLFramebufferInfo m_info;
        SkColorType m_colorType;
		SkSurfaceProps m_props;
    };
}

#endif // PTK_PLATFORM_GLFW_GLCONTEXT_HPP
