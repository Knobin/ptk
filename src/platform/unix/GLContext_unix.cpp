//
//  platform/win/WinRasterContext.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-20.
//

// pTK Headers
#include "ptk/platform/unix/GLContext_unix.hpp"
#include "ptk/platform/unix/ApplicationHandle_unix.hpp"
#include "ptk/Log.hpp"
#include "ptk/core/Exception.hpp"

// C++ Headers
#include <optional>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/gl/GrGLInterface.h"
#include "src/gpu/ganesh/GrCaps.h"
#include "src/gpu/ganesh/GrDirectContextPriv.h"
#include "src/gpu/ganesh/gl/GrGLDefines_impl.h"
#include "src/gpu/ganesh/gl/GrGLUtil.h"
#include "src/gpu/gl/GrGLDefines.h"
PTK_DISABLE_WARN_END()

using App = pTK::ApplicationHandle_unix;
typedef GLXContext (*glXCreateContextAttribsARBProc)(Display*, GLXFBConfig, GLXContext, Bool, const int*);

///////////////////////////////////////////////////////////////////////////////////////////////////

// Helper to check for extension string presence.  Adapted from:
//   http://www.opengl.org/resources/features/OGLextensions/
static bool isExtensionSupported(const char *extList, const char *extension) {
    const char *start;
    const char *where, *terminator;

    where = strchr(extension, ' ');
    if (where || *extension == '\0') {
        return false;
    }

    for (start=extList;;) {
        where = strstr(start, extension);

        if (!where) {
            break;
        }

        terminator = where + strlen(extension);

        if ( where == start || *(where - 1) == ' ' ) {
            if ( *terminator == ' ' || *terminator == '\0' ) {
                return true;
            }
        }

        start = terminator;
    }

    return false;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace pTK
{
    static std::pair<GLint, GLint> GLXVersion(Display *display)
    {
        GLint major{0}, minor{0};
        glXQueryVersion(display, &major, &minor);
        return {minor, major};
    }

    static std::optional<GLXFBConfig> GetFbc(Display *display, int screenID, GLint t_attr[])
    {
        int fbcount;
        GLXFBConfig* fbc = glXChooseFBConfig(display, screenID, t_attr, &fbcount);
        if (fbc == nullptr)
        {
            return std::nullopt;
        }
        PTK_INFO("Found {} matching framebuffers", fbcount);

        // Pick the FB config/visual with the most samples per pixel
        PTK_INFO("Getting best XVisualInfo");
        int best_fbc = -1, worst_fbc = -1, best_num_samp = -1, worst_num_samp = 999;
        for (int i = 0; i < fbcount; ++i)
        {
            XVisualInfo *vi = glXGetVisualFromFBConfig(display, fbc[i] );
            if ( vi != nullptr)
            {
                int samp_buf, samples;
                glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLE_BUFFERS, &samp_buf);
                glXGetFBConfigAttrib(display, fbc[i], GLX_SAMPLES, &samples);
;
                PTK_INFO("\tMatching fbconfig {0} visual ID {1:x}: SAMPLE_BUFFERS = {2}, SAMPLES = {3}", i, vi->visualid, samp_buf, samples);

                if ( best_fbc < 0 || (samp_buf && samples > best_num_samp) ) {
                    best_fbc = i;
                    best_num_samp = samples;
                }
                if ( worst_fbc < 0 || !samp_buf || samples < worst_num_samp )
                    worst_fbc = i;
                worst_num_samp = samples;
            }
            XFree(vi);
        }
        PTK_INFO("Best visual info index: {}", best_fbc);
        GLXFBConfig bestFbc = fbc[best_fbc];
        XFree(fbc);

        return bestFbc;
    }

    static GLXContext CreateGLXContext(Display *display, int screenID, const GLXFBConfig& fbc)
    {
        // Create GLX OpenGL context
        glXCreateContextAttribsARBProc glXCreateContextAttribsARB{0};
        glXCreateContextAttribsARB = reinterpret_cast<glXCreateContextAttribsARBProc>(glXGetProcAddressARB(reinterpret_cast<const GLubyte*>("glXCreateContextAttribsARB")));

        const char *glxExts{glXQueryExtensionsString(display, screenID)};
        PTK_INFO("Late extensions: {}", glxExts);
        if (glXCreateContextAttribsARB == 0)
        {
            PTK_WARN("glXCreateContextAttribsARB() not found");
        }

        int context_attribs[] = {
            GLX_CONTEXT_MAJOR_VERSION_ARB, 3,
            GLX_CONTEXT_MINOR_VERSION_ARB, 2,
            GLX_CONTEXT_FLAGS_ARB, GLX_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
            x11::None
        };

        GLXContext context{0};
        if (!isExtensionSupported( glxExts, "GLX_ARB_create_context")) {
            context = glXCreateNewContext(display, fbc, GLX_RGBA_TYPE, 0, True);
        }
        else {
            context = glXCreateContextAttribsARB(display, fbc, 0, true, context_attribs);
        }
        XSync(display, False);

        return context;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    GLContext_unix::GLContext_unix(::Window *window, const Size& size)
        : ContextBase(size), m_window{window}, m_context{nullptr},
            m_GrContextOptions{}, m_props{0, kRGB_H_SkPixelGeometry}
    {
        Display *display{App::Display()};
        int screenID{App::Screen()};

        auto[minorGLX, majorGLX] = GLXVersion(display);
        if (majorGLX <= 1 && minorGLX < 2)
        {
            throw ContextError("GLX 1.2 or greater is required");
        }

        PTK_INFO("GLX client version: {}", glXGetClientString(display, GLX_VERSION));
        PTK_INFO("GLX client vendor: " , glXGetClientString(display, GLX_VENDOR));
        PTK_INFO("GLX client extensions:", glXGetClientString(display, GLX_EXTENSIONS));
        PTK_INFO("GLX server version: ", glXQueryServerString(display, screenID, GLX_VERSION));
        PTK_INFO("GLX server vendor: ", glXQueryServerString(display, screenID, GLX_VENDOR));
        PTK_INFO("GLX server extensions: ", glXQueryServerString(display, screenID, GLX_EXTENSIONS));

        GLint glxAttribs[] = {
                GLX_X_RENDERABLE    , True,
                GLX_DRAWABLE_TYPE   , GLX_WINDOW_BIT,
                GLX_RENDER_TYPE     , GLX_RGBA_BIT,
                GLX_X_VISUAL_TYPE   , GLX_TRUE_COLOR,
                GLX_RED_SIZE        , 8,
                GLX_GREEN_SIZE      , 8,
                GLX_BLUE_SIZE       , 8,
                GLX_ALPHA_SIZE      , 8,
                GLX_DEPTH_SIZE      , 24,
                GLX_STENCIL_SIZE    , 8,
                GLX_DOUBLEBUFFER    , True,
                x11::None
        };

        std::optional<GLXFBConfig> bestFbc{GetFbc(display, screenID, glxAttribs)};
        if (!bestFbc)
            throw ContextError("Failed to retrieve framebuffer.");

        m_visual = glXGetVisualFromFBConfig(display, *bestFbc);
        if (m_visual == 0)
            throw ContextError("Could not create correct visual window");
        if (screenID != m_visual->screen)
            throw ContextError("screenId(" + std::to_string(screenID) + ") does not match visual->screen(" + std::to_string(m_visual->screen) + ")");

        m_GLContext = CreateGLXContext(display, screenID, *bestFbc);
        if (!glXIsDirect(display, m_GLContext)) {
            PTK_INFO("Indirect GLX rendering context obtained");
        }
        else {
            PTK_INFO("Direct GLX rendering context obtained");
        }
        glXMakeCurrent(display, *m_window, m_GLContext);

        auto glInterface = GrGLMakeNativeInterface();
		PTK_ASSERT(glInterface, "Failed to create interface!");
		m_backendContext.reset(glInterface.release());

        PTK_INFO("GL Vendor: {}", glGetString(GL_VENDOR));
        PTK_INFO("GL Renderer: {}", glGetString(GL_RENDERER));
        PTK_INFO("GL Version: {}", glGetString(GL_VERSION));
        PTK_INFO("GL Shading Language: {}", glGetString(GL_SHADING_LANGUAGE_VERSION));

        m_context = GrDirectContext::MakeGL(m_backendContext, m_GrContextOptions);

        resize(size);
    }

    GLContext_unix::~GLContext_unix()
    {
        // Apparently, surface needs to be destroyed before context.
        // Otherwise, SkRefCount will give a nice assert.
        m_surface.reset();
        m_context.reset();

        PTK_INFO("Destroyed GLContext_unix");
    }

    void GLContext_unix::resize(const Size& size)
    {
        if (m_context)
        {
            GrGLint buffer;
            GR_GL_CALL(m_backendContext.get(), GetIntegerv(GR_GL_FRAMEBUFFER_BINDING, &buffer));

            GrGLFramebufferInfo fbInfo;
            fbInfo.fFBOID = static_cast<GrGLuint>(buffer);
            fbInfo.fFormat = GR_GL_RGBA8;

            int width{static_cast<int>(size.width)};
            int height{static_cast<int>(size.height)};

            glViewport(0, 0, width, height);

            GrBackendRenderTarget backendRenderTarget(width, height, 1, 8, fbInfo);

            SkSurface* surface{ SkSurface::MakeFromBackendRenderTarget(m_context.get(), backendRenderTarget,
                    kBottomLeft_GrSurfaceOrigin, kRGBA_8888_SkColorType, nullptr, &m_props).release() };
            PTK_ASSERT(surface, "Failed to create surface!");
            m_surface.reset(surface);

            //clear(Color{0xFFFFFFFF});
            PTK_INFO("Sized GLContext_unix to {}x{}", size.width, size.height);
            setSize(size);
        }
    }

    sk_sp<SkSurface> GLContext_unix::surface() const
    {
        return m_surface;
    }

    void GLContext_unix::swapBuffers()
    {
        glXSwapBuffers(App::Display(), *m_window);
    }
}
