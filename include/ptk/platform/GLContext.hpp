//
//  platform/GLContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

#ifndef PTK_PLATFORM_GLCONTEXT_HPP
#define PTK_PLATFORM_GLCONTEXT_HPP

// Local Headers
#include "ptk/core/ContextBase.hpp"

// C++ Headers
#include <memory>

// OpenGL Headers
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl.h>
#else
	#ifdef _WIN32
	#define NOMINMAX
		#include <windows.h>
	#endif
	#include <GL/gl.h>
#endif

// Skia Headers
#include "include/gpu/GrContext.h"

namespace pTK
{
    /** GLContext class implementation.
     
        Canvas for setting up OpenGL rendering.
    */
    class GLContext : public ContextBase
    {
    public:
        /** Constructs GLContext with default values.
         
            @return    default initialized GLContext
        */
        GLContext(const Size& size);

        /** Deconstructor for GLContext.

        */
        virtual ~GLContext();
        
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

#endif // PTK_PLATFORM_GLCONTEXT_HPP
