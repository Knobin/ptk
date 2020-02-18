//
//  platform/GLCanvas.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

#ifndef PTK_PLATFORM_GLCANVAS_HPP
#define PTK_PLATFORM_GLCANVAS_HPP

// Local Headers
#include "ptk/core/CanvasBase.hpp"

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
    /** GLCanvas class implementation.
     
        Canvas for setting up OpenGL rendering.
    */
    class GLCanvas : public CanvasBase
    {
    public:
        /** Constructs GLCanvas with default values.
         
            @return    default initialized GLCanvas
        */
        GLCanvas(const Size& size);

        /** Deconstructor for GLCanvas.

        */
        virtual ~GLCanvas();
        
        /** Function for resizing the Canvas.
         
            @param width   New width of canvas
            @param width   New height of canvas
        */
        void resize(const Size& size) override;
        
    private:
        // Skia
        Ref<GrContext> m_context;
        GrGLFramebufferInfo m_info;
        SkColorType m_colorType;
		SkSurfaceProps m_props;
    };
}

#endif // PTK_PLATFORM_GLCANVAS_HPP
