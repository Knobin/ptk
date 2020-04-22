//
//  platform/win/WinRasterContext.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-20.
//

// Local Headers
#include "ptk/platform/win/WinGLContext.hpp"
#include "ptk/Core.hpp"

// OpenGL Windows Headers
#include <GL/gl.h>

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
//#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "src/gpu/gl/GrGLUtil.h"
#include "src/utils/win/SkWGL.h"
#ifdef PTK_COMPILER_GCC
#pragma GCC diagnostic pop
#elif PTK_COMPILER_CLANG
#pragma clang diagnostic pop
#elif PTK_COMPILER_MSVC
#pragma warning( pop )
#endif

namespace pTK
{
    WinGLContext::WinGLContext(HWND hwnd, const Size& size)
        : ContextBase(size), m_hwnd{hwnd}, m_hglrc{}, m_context{nullptr}, m_info{}, m_colorType{},
            m_props{SkSurfaceProps::kUseDeviceIndependentFonts_Flag, SkSurfaceProps::kLegacyFontHost_InitType}
    {
        HDC dc{GetDC(hwnd)};
        m_hglrc = SkCreateWGLContext(dc, 1, false, kGLPreferCompatibilityProfile_SkWGLContextRequest);
        PTK_ASSERT(m_hglrc, "Failed to create OpenGL handle!");

        if (wglMakeCurrent(dc, m_hglrc))
        {
            glClearStencil(0);
            glClearColor(0, 0, 0, 0);
            glStencilMask(0xFFFFFFFF);
            glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

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
#ifdef PTK_DEBUG
        else
        {
            PTK_ASSERT(false, "Failed the wglMakeCurrent function!");
        }
#endif
    }

    WinGLContext::~WinGLContext()
    {
        // Apparently, surface needs to be destroyed before context.
        // Otherwise, SkRefCount will give a nice assert.
        m_surface.reset();
        m_context.reset();
    }

    void WinGLContext::resize(const Size& size)
    {
        glViewport(0, 0, size.width, size.height);

        GrBackendRenderTarget backendRenderTarget(size.width, size.height, 1, 8, m_info);

        SkSurface *surface = SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, m_colorType, nullptr, &m_props).release();
        PTK_ASSERT(surface, "Failed to create surface!");
        m_surface.reset(surface);

        PTK_INFO("Created WinGLCanvas: {}x{}", size.width, size.height);
        setSize(size);
    }

    SkCanvas* WinGLContext::skCanvas() const
    {
        return m_surface->getCanvas();
    }

    SkSurface* WinGLContext::skSurface() const
    {
        return m_surface.get();
    }

    void WinGLContext::swapBuffers()
    {
        HDC dc{GetDC(m_hwnd)};
        SwapBuffers(dc);
        ReleaseDC(m_hwnd, dc);
    }
}
