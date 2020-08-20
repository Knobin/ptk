//
//  platform/glfw/GLContext_glfw.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

// Local Headers
#include "GLContext_glfw.hpp"
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
    GLContext_glfw::GLContext_glfw(const Size& size)
        : ContextBase(size), m_context{nullptr}, m_info{}, m_colorType{},
            m_props{SkSurfaceProps::kUseDeviceIndependentFonts_Flag, SkSurfaceProps::kLegacyFontHost_InitType}
    {
		auto glInterface = GrGLMakeNativeInterface();
		PTK_ASSERT(glInterface, "Failed to create interface!");
		m_context.reset(GrContext::MakeGL(glInterface).release());

		GrGLint buffer{};
		GR_GL_GetIntegerv(glInterface.get(), GR_GL_FRAMEBUFFER_BINDING, &buffer);
		GrGLFramebufferInfo info{};
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

    GLContext_glfw::~GLContext_glfw()
    {
        // Apparently, surface needs to be destroyed before context.
        // Otherwise, SkRefCount will give a nice assert.
        m_surface.reset();
        m_context.reset();
    }

    void GLContext_glfw::resize(const Size& size)
    {
        glViewport(0, 0, size.width, size.height);

        GrBackendRenderTarget backendRenderTarget(size.width, size.height, 1, 8, m_info);

        SkSurface *surface{SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget, kBottomLeft_GrSurfaceOrigin, m_colorType, nullptr, &m_props).release()};
        PTK_ASSERT(surface, "Failed to create surface!");

        PTK_INFO("Resized GLContext {}x{}", size.width, size.height);
        m_surface.reset(surface);

        setSize(size);
    }

    SkCanvas* GLContext_glfw::skCanvas() const
    {
        return m_surface->getCanvas();
    }

    SkSurface* GLContext_glfw::skSurface() const
    {
        return m_surface.get();
    }
}
