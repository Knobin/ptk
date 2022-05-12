//
//  platform/win/WinRasterContext.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-20.
//

// Local Headers
#include "ptk/platform/win/GLContext_win.hpp"

// OpenGL Windows Headers
#include <GL/gl.h>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/gpu/gl/GrGLInterface.h"
#include "src/gpu/ganesh/GrCaps.h"
#include "src/gpu/ganesh/GrDirectContextPriv.h"
#include "src/gpu/ganesh/gl/GrGLDefines_impl.h"
#include "src/gpu/ganesh/gl/GrGLUtil.h"
#include "src/gpu/gl/GrGLDefines.h"
#include "src/utils/win/SkWGL.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    GLContext_win::GLContext_win(HWND hwnd, const Size& size)
        : ContextBase(size), m_hwnd{ hwnd }, m_hglrc{}, m_backendContext{nullptr}, m_context{nullptr}, 
            m_GrContextOptions{}, m_props{0, kRGB_H_SkPixelGeometry}
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
            m_backendContext.reset(GrGLMakeNativeInterface().release());

            // m_info.fFormat = GL_RGBA8;
            // m_colorType = kRGBA_8888_SkColorType;

            m_context = GrDirectContext::MakeGL(m_backendContext, m_GrContextOptions);

            PTK_INFO("Initialized GLContext_win");
            resize(size);
        }
#ifdef PTK_DEBUG
        else
        {
            PTK_ASSERT(false, "Failed the wglMakeCurrent function!");
        }
#endif
    }

    GLContext_win::~GLContext_win()
    {
        // Apparently, surface needs to be destroyed before context.
        // Otherwise, SkRefCount will give a nice assert.
        m_surface.reset();
        m_context.reset();

        PTK_INFO("Destroyed GLContext_win");
    }

    void GLContext_win::resize(const Size& size)
    {
        if (m_context)
        {
            GrGLint buffer;
            GR_GL_CALL(m_backendContext.get(), GetIntegerv(GR_GL_FRAMEBUFFER_BINDING, &buffer));

            GrGLFramebufferInfo fbInfo;
            fbInfo.fFBOID = buffer;
            fbInfo.fFormat = GR_GL_RGBA8;

            glViewport(0, 0, size.width, size.height);

            GrBackendRenderTarget backendRenderTarget(size.width, size.height, 1, 8, fbInfo);

            SkSurface* surface{ SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget,
                    kBottomLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType, nullptr, &m_props).release() };
            PTK_ASSERT(surface, "Failed to create surface!");
            m_surface.reset(surface);

            //clear(Color{0xFFFFFFFF});
            PTK_INFO("Sized GLContext_win to {}x{}", size.width, size.height);
            setSize(size);
        }
    }

    sk_sp<SkSurface> GLContext_win::surface() const
    {
        return m_surface;
    }

    void GLContext_win::swapBuffers()
    {
        HDC dc{GetDC(m_hwnd)};
        SwapBuffers(dc);
        ReleaseDC(m_hwnd, dc);
    }
}
