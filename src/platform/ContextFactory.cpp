//
//  src/platform/ContextFactory.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// Local Headers
#include "../include/Log.hpp"

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/platform/ContextFactory.hpp"

namespace pTK::Platform
{
    namespace ContextFactoryImpl
    {
        // Availability check.
        bool IsContextAvailable(ContextBackendType type);

        // Best match creator.
        std::unique_ptr<ContextBase> MakeContext(Window*, const Size&, const Vec2f&, const WindowInfo&);

        // Raster.
        std::unique_ptr<ContextBase> MakeRasterContext(Window*, const Size&, const Vec2f&);

        // OpenGL (Windows, Unix).
#ifdef PTK_OPENGL
        std::unique_ptr<ContextBase> MakeGLContext(Window*, const Size&, const Vec2f&);
#endif

        // Metal (macOS).
#ifdef PTK_METAL
        std::unique_ptr<ContextBase> MakeMetalContext(Window*, const Size&, const Vec2f&);
#endif
    } // namespace ContextFactoryImpl

    std::unique_ptr<ContextBase> ContextFactory::Make(Window* window, const Size& size, const Vec2f& scale,
                                                      const WindowInfo& info)
    {
        return ContextFactoryImpl::MakeContext(window, size, scale, info);
    }

    std::unique_ptr<ContextBase> ContextFactory::MakeRaster(Window* window, const Size& size, const Vec2f& scale)
    {
        return ContextFactoryImpl::MakeRasterContext(window, size, scale);
    }

    std::unique_ptr<ContextBase> ContextFactory::MakeGL([[maybe_unused]] Window* window,
                                                        [[maybe_unused]] const Size& size,
                                                        [[maybe_unused]] const Vec2f& scale)
    {
#ifdef PTK_OPENGL
        return ContextFactoryImpl::MakeGLContext(window, size, scale);
#else
        PTK_ERROR("OpenGL context is not available on this platform");
        return nullptr;
#endif
    }

    std::unique_ptr<ContextBase> ContextFactory::MakeMetal([[maybe_unused]] Window* window,
                                                           [[maybe_unused]] const Size& size,
                                                           [[maybe_unused]] const Vec2f& scale)
    {
#ifdef PTK_METAL
        return ContextFactoryImpl::MakeMetalContext(window, size, scale);
#else
        PTK_ERROR("Metal context is not available on this platform");
        return nullptr;
#endif
    }

    bool ContextFactory::IsAvailable(ContextBackendType type)
    {
        return ContextFactoryImpl::IsContextAvailable(type);
    }

} // namespace pTK::Platform
