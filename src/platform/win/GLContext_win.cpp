//
//  platform/win/WinRasterContext.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-20.
//

// Local Headers
#include "GLContext_win.hpp"
#include "../../Log.hpp"
#include "../../core/Assert.hpp"

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

namespace pTK::Platform
{
    GLContext_win::GLContext_win(HWND hwnd, const Size& size)
        : ContextBase(ContextBackendType::GL, size),
          m_hwnd{hwnd},
          m_hglrc{},
          m_backendContext{nullptr},
          m_context{nullptr},
          m_GrContextOptions{},
          m_props{0, kRGB_H_SkPixelGeometry}
    {
        createContext(size);
        PTK_INFO("Initialized GLContext_win");
    }

    GLContext_win::~GLContext_win()
    {
        destroyContext();
        PTK_INFO("Destroyed GLContext_win");
    }

    void GLContext_win::resize(const Size& size)
    {
        if (m_context && m_backendContext)
        {
            GrGLint buffer;
            GR_GL_CALL(m_backendContext.get(), GetIntegerv(GR_GL_FRAMEBUFFER_BINDING, &buffer));

            GrGLFramebufferInfo fbInfo;
            fbInfo.fFBOID = buffer;
            fbInfo.fFormat = GR_GL_RGBA8;

            const int width{static_cast<int>(size.width)};
            const int height{static_cast<int>(size.height)};
            GrBackendRenderTarget backendRenderTarget(width, height, m_sampleCount, m_stencilBits, fbInfo);

            m_surface = SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget,
                                                               kBottomLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType,
                                                               nullptr, &m_props);
            PTK_ASSERT(m_surface, "Failed to create surface!");

            // clear(Color{0xFFFFFFFF});
            setSize(size);
            PTK_INFO("Sized GLContext_win to {}x{}", size.width, size.height);
        }
#ifdef PTK_DEBUG
        else
        {
            PTK_ASSERT(false, "Failed to resize GLContext_win, context is nullptr!");
        }
#endif
    }

    sk_sp<SkSurface> GLContext_win::surface() const
    {
        return m_surface;
    }

    void GLContext_win::swapBuffers()
    {
        // PTK_INFO("swapBuffers");
        HDC dc = GetDC((HWND)m_hwnd);
        SwapBuffers(dc);
        ReleaseDC((HWND)m_hwnd, dc);
    }

    void GLContext_win::createContext(const Size& size)
    {
        HDC dc{GetDC(m_hwnd)};
        m_hglrc = SkCreateWGLContext(dc, 1, false, kGLPreferCompatibilityProfile_SkWGLContextRequest);
        PTK_ASSERT(m_hglrc, "Failed to create OpenGL handle!");
        PTK_INFO("Created OpenGL context using WGL.");

        SkWGLExtensions extensions;
        if (extensions.hasExtension(dc, "WGL_EXT_swap_control"))
            extensions.swapInterval(1);

        if (wglMakeCurrent(dc, m_hglrc))
        {
            glClearStencil(0);
            glClearColor(0, 0, 0, 0);
            glStencilMask(0xFFFFFFFF);
            glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

            int pixelFormat = GetPixelFormat(dc);
            PIXELFORMATDESCRIPTOR pfd;
            DescribePixelFormat(dc, pixelFormat, sizeof(pfd), &pfd);
            m_stencilBits = pfd.cStencilBits;

            if (extensions.hasExtension(dc, "WGL_ARB_multisample"))
            {
                static const int kSampleCountAttr = SK_WGL_SAMPLES;
                extensions.getPixelFormatAttribiv(dc, pixelFormat, 0, 1, &kSampleCountAttr, &m_sampleCount);
                m_sampleCount = std::max(m_sampleCount, 1);
            }

            auto glInterface = GrGLMakeNativeInterface();
            PTK_ASSERT(glInterface, "Failed to create interface!");
            m_backendContext.reset(glInterface.release());
            PTK_INFO("Created GrGLInterface");

            // m_info.fFormat = GL_RGBA8;
            // m_colorType = kRGBA_8888_SkColorType;

            m_context = GrDirectContext::MakeGL(m_backendContext, m_GrContextOptions);
            PTK_INFO("Created GrDirectContext");

            resize(size);
        }
#ifdef PTK_DEBUG
        else
        {
            PTK_ASSERT(false, "Failed the wglMakeCurrent function!");
        }
#endif
    }

    void GLContext_win::destroyContext()
    {
        m_surface.reset(nullptr);
        if (m_context)
        {
            m_context->abandonContext();
            m_context.reset();
        }

        m_backendContext.reset(nullptr);

        wglDeleteContext(m_hglrc);
        m_hglrc = nullptr;
    }
} // namespace pTK
