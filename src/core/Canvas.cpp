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
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/gpu/GrRenderTarget.h"
#pragma GCC diagnostic pop

namespace pTK
{
    Canvas::Canvas(const Vec2<uint32_t>& size)
        : m_context{nullptr}, m_surface{nullptr}, m_canvas{nullptr}, m_info{}, m_colorType{}
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
        PTK_ASSERT(m_surface, "Failed to create surface!");
    }
    
    Canvas::Canvas(const Canvas& canvas, const Vec2<uint32_t>& size)
        : m_context{canvas.m_context}, m_surface{nullptr}, m_canvas{nullptr}, m_info{canvas.m_info}, m_colorType{canvas.m_colorType}
    {
        GrBackendRenderTarget backendRenderTarget(size.x, size.y, 0, 0, m_info);
        
        m_surface = SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, m_colorType, nullptr, nullptr).release();
        
        resize(size);
        PTK_ASSERT(m_surface, "Failed to generate surface!");
    }
    
    Canvas::~Canvas()
    {
        delete m_surface;
    }
    
    void Canvas::resize(const Vec2<uint32_t>& size)
    {
        glViewport(0, 0, size.x, size.y);
        
        GrBackendRenderTarget backendRenderTarget(size.x, size.y, 0, 0, m_info);
        
        delete m_surface;
        m_surface = SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, m_colorType, nullptr, nullptr).release();
        
        if (m_surface == nullptr)
            throw std::logic_error("Skia error");
        
        // Save Canvas for rendering.
        m_canvas = m_surface->getCanvas();
        
        clear();
        
        PTK_INFO("[Canvas] Created with w: {0:d}px and h: {1:d}px", size.x, size.y);
    }
    
    void Canvas::clear()
    {
        m_canvas->clear(SkColorSetARGB(255, 255*0.3, 255*0.2, 255*0.2));
        
        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    
    SkCanvas* Canvas::skCanvas() const
    {
        return m_canvas;
    }
    
    SkSurface* Canvas::skSurface() const
    {
        return m_surface;
    }
}
