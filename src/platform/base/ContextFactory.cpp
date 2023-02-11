//
//  platform/base/ContextHandle.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// pTK Headers
#include "ptk/platform/base/ContextFactory.hpp"
#include "ptk/Log.hpp"
#include "ptk/core/Defines.hpp"

namespace pTK::Platform
{
    std::unique_ptr<ContextBase> ContextFactory::Make(Window* window, const Size& size, const Vec2f& scale,
                                                      const WindowInfo& info)
    {
        extern std::unique_ptr<ContextBase> MakeContext(Window*, const Size&, const Vec2f&, const WindowInfo&);
        return MakeContext(window, size, scale, info);
    }

    std::unique_ptr<ContextBase> ContextFactory::MakeRaster(Window* window, const Size& size, const Vec2f& scale)
    {
        extern std::unique_ptr<ContextBase> MakeRasterContext(Window*, const Size&, const Vec2f&);
        return MakeRasterContext(window, size, scale);
    }

    std::unique_ptr<ContextBase> ContextFactory::MakeGL([[maybe_unused]] Window* window,
                                                        [[maybe_unused]] const Size& size,
                                                        [[maybe_unused]] const Vec2f& scale)
    {
#ifdef PTK_OPENGL
        extern std::unique_ptr<ContextBase> MakeGLContext(Window*, const Size&, const Vec2f&);
        return MakeGLContext(window, size, scale);
#endif
        PTK_ERROR("OpenGL context is not available on this platform");
        return nullptr;
    }

    std::unique_ptr<ContextBase> ContextFactory::MakeMetal(Window* window, const Size& size, const Vec2f& scale)
    {
#ifdef PTK_METAL
        extern std::unique_ptr<ContextBase> MakeMetalContext(Window*, const Size&, const Vec2f&);
        return MakeMetalContext(window, size, scale);
#endif
        PTK_ERROR("Metal context is not available on this platform");
        return nullptr;
    }

    bool ContextFactory::IsAvailable(ContextBackendType type)
    {
        extern bool IsContextAvailable(ContextBackendType type);
        return IsContextAvailable(type);
    }

} // namespace pTK::Platform
