//
//  core/Canvas.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

// Local Headers
#include "ptk/core/Canvas.hpp"
#include "ptk/Log.hpp"

// Skia Headers
#ifdef PTK_COMPILER_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#elif PTK_COMPILER_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#endif
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/gpu/GrRenderTarget.h"
#ifdef PTK_COMPILER_GCC
    #pragma GCC diagnostic pop
#elif PTK_COMPILER_CLANG
    #pragma clang diagnostic pop
#endif

namespace pTK
{
    Canvas::Canvas(const Size& size)
        : NonMovable(), NonCopyable(), m_context{nullptr}, m_surface{nullptr}, m_canvas{nullptr}, m_info{},
            m_colorType{}, m_size{size}
    {
        auto interface = GrGLMakeNativeInterface();
        m_context.reset(GrContext::MakeGL(interface).release());

        GrGLint buffer;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &buffer);
        m_info.fFBOID = (GrGLuint) buffer;
        m_info.fFormat = GL_RGBA8;
        
        if (kRGBA_8888_GrPixelConfig == kSkia8888_GrPixelConfig)
            m_colorType = kRGBA_8888_SkColorType;
        else
            m_colorType = kBGRA_8888_SkColorType;
            
        resize(size);
    }
    
    Canvas::~Canvas()
    {
    }
    
    void Canvas::resize(const Size& size)
    {
        glViewport(0, 0, size.width, size.height);
        
        GrBackendRenderTarget backendRenderTarget(size.width, size.height, 0, 0, m_info);
        
        m_surface = SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, m_colorType, nullptr, nullptr);
        
        PTK_ASSERT(m_surface, "Failed to create surface!");
        
        // Save Canvas for rendering.
        m_canvas = m_surface->getCanvas();
        m_size = size;
        
        PTK_INFO("Created Canvas, {0}x{1}", size.width, size.height);
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
        return SkPoint{pos.x*scale.x, pos.y*scale.y};
    }
    
    SkPoint convertToSkPoint(const Size& size, const Vec2f& scale)
    {
        return SkPoint{size.width*scale.x, size.height*scale.y};
    }
}
