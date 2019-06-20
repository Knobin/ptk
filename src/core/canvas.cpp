//
//  core/canvas.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

// Local Headers
#include "ptk/core/canvas.hpp"
#include "ptk/log.hpp"

// Skia
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "include/gpu/GrRenderTarget.h"

namespace pTK
{
    Canvas::Canvas(unsigned int width, unsigned int height)
        : m_context{nullptr}, m_surface{nullptr}, m_canvas{nullptr}, m_info{}, m_color_type{}
    {
        auto interface = GrGLMakeNativeInterface();
        m_context = GrContext::MakeGL(interface).release();

        GrGLint buffer;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &buffer);
        m_info.fFBOID = (GrGLuint) buffer;
        m_info.fFormat = GL_RGBA8;
        
        if (kRGBA_8888_GrPixelConfig == kSkia8888_GrPixelConfig)
            m_color_type = kRGBA_8888_SkColorType;
        else
            m_color_type = kBGRA_8888_SkColorType;
            
        resize(width, height);
        clear();
        
        PTK_INFO("[Canvas] Created with w: {0:d}px and h: {0:d}px", width, height);
    }
    
    Canvas::~Canvas()
    {
        delete m_surface;
        delete m_context;
    }
    
    void Canvas::resize(unsigned int width, unsigned int height)
    {
        glViewport(0, 0, width, height);
        
        GrBackendRenderTarget backendRenderTarget(width, height, 0, 0, m_info);
        
        delete m_surface;
        m_surface = SkSurface::MakeFromBackendRenderTarget(m_context, backendRenderTarget, kBottomLeft_GrSurfaceOrigin, m_color_type, nullptr, nullptr).release();
        
        if (m_surface == nullptr)
            throw std::logic_error("Skia error");
        
        // Save Canvas for rendering.
        m_canvas = m_surface->getCanvas();
    }
    
    void Canvas::clear()
    {
        m_canvas->clear(SkColorSetARGB(255, 255*0.3, 255*0.2, 255*0.2));
        
        glClearColor(0.3f, 0.2f, 0.2f, 1.0f);
        glClearStencil(0);
        glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    }
    
    SkCanvas* Canvas::skcanvas() const
    {
        return m_canvas;
    }
}
