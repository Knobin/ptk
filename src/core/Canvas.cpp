//
//  core/Canvas.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

// Local Headers
#include "ptk/core/Canvas.hpp"
#include "ptk/Core.hpp"

// Skia Headers
#ifdef PTK_COMPILER_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#elif PTK_COMPILER_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#elif PTK_COMPILER_MSVC
	#pragma warning( push, 0 )
#endif
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "src/gpu/gl/GrGLUtil.h"
#ifdef PTK_COMPILER_GCC
    #pragma GCC diagnostic pop
#elif PTK_COMPILER_CLANG
    #pragma clang diagnostic pop
#elif PTK_COMPILER_MSVC
	#pragma warning( pop )
#endif

namespace pTK
{
    Canvas::Canvas(const Size& size)
        : Singleton(), m_context{nullptr}, m_surface{nullptr}, m_canvas{nullptr}, m_info{},
			m_colorType{}, m_props{SkSurfaceProps::kUseDeviceIndependentFonts_Flag, SkSurfaceProps::kLegacyFontHost_InitType}, m_size{size}
    {
		auto glInterface = GrGLMakeNativeInterface();
		PTK_ASSERT(glInterface, "Failed to create interface!");
		m_context.reset(GrContext::MakeGL(glInterface).release());

		GrGLint buffer;
		GR_GL_GetIntegerv(glInterface.get(), GR_GL_FRAMEBUFFER_BINDING, &buffer);
		GrGLFramebufferInfo info;
		info.fFBOID = (GrGLuint)buffer;
		m_info.fFormat = GL_RGBA8;

#ifdef PTK_COMPILER_MSVC
#pragma warning( push )
#pragma warning( disable : 4127)
#endif
		if (kRGBA_8888_GrPixelConfig == kSkia8888_GrPixelConfig)
			m_colorType = kRGBA_8888_SkColorType;
		else
			m_colorType = kBGRA_8888_SkColorType;
#ifdef PTK_COMPILER_MSVC
#pragma warning (pop )
#endif
        resize(size);
    }
    
    Canvas::~Canvas()
    {
    }
    
    void Canvas::resize(const Size& size)
    {
        glViewport(0, 0, size.width, size.height);
        
        GrBackendRenderTarget backendRenderTarget(size.width, size.height, 1, 8, m_info);

        m_surface = SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, m_colorType, nullptr, &m_props);
        PTK_ASSERT(m_surface, "Failed to create surface!");
        
        // Save Canvas for rendering.
        m_canvas = m_surface->getCanvas();
        m_size = size;
    }
    
    void Canvas::clear(const Color& color)
    {
        m_canvas->clear(SkColorSetARGB(255, color.r, color.g, color.b));
    }
    
    SkCanvas* Canvas::skCanvas() const
    {
        return m_canvas;
    }
    
    SkSurface* Canvas::skSurface() const
    {
        return m_surface.get();
    }
    
    const Size& Canvas::getSize() const
    {
        return m_size;
    }
    
    // Functions for converting utility classes to SkPoint for drawing.
    SkPoint convertToSkPoint(const Point& pos, const Vec2f& scale)
    {
        return SkPoint{static_cast<float>(pos.x)*scale.x, static_cast<float>(pos.y)*scale.y};
    }
    
    SkPoint convertToSkPoint(const Size& size, const Vec2f& scale)
    {
        return SkPoint{static_cast<float>(size.width)*scale.x, static_cast<float>(size.height)*scale.y};
    }
}
