//
//  platform/unix/ContextFactory_unix.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-20.
//

// Local Headers
#include "../../Log.hpp"
#include "../../core/Assert.hpp"
#include "RasterContext_unix.hpp"
#include "WindowHandle_unix.hpp"

// Include GL backend.
#ifdef PTK_OPENGL
    #include "GLContext_unix.hpp"
#endif

// pTK Headers
#include "ptk/Window.hpp"
#include "ptk/core/Defines.hpp"
#include "ptk/platform/base/ContextFactory.hpp"
#include "ptk/platform/common/RasterContext.hpp"

namespace pTK::Platform::ContextFactoryImpl
{
    static Size ScaleSize(const Size& size, const Vec2f& scale)
    {
#ifdef PTK_DEBUG
        if (scale.x != scale.y)
            PTK_WARN("Context scale is not the same, x: {}, y: {}", static_cast<double>(scale.x),
                     static_cast<double>(scale.y));
#endif
        return {static_cast<Size::value_type>(static_cast<float>(size.width) * scale.x),
                static_cast<Size::value_type>(static_cast<float>(size.height) * scale.y)};
    }

    std::unique_ptr<ContextBase> MakeRasterContext(Window* window, const Size& size, const Vec2f& scale)
    {
        // Software backend is always available.
        auto handle = dynamic_cast<WindowHandle_unix*>(window->platformHandle());
        return std::make_unique<RasterContext_unix>(handle->xWindow(), ScaleSize(size, scale), handle->xVisualInfo());
    }

    std::unique_ptr<ContextBase> MakeGLContext(Window* window, const Size& size, const Vec2f& scale)
    {
#ifdef PTK_OPENGL
        auto handle = dynamic_cast<WindowHandle_unix*>(window->platformHandle());
        return std::make_unique<GLContext_unix>(handle->xWindow(), ScaleSize(size, scale));
#else
        return nullptr;
#endif
    }

    std::unique_ptr<ContextBase> MakeContext(Window* window, const Size& size, const Vec2f& scale,
                                             const WindowInfo& info)
    {
#ifdef PTK_OPENGL
        if (info.backend == WindowInfo::Backend::Hardware)
            return MakeGLContext(window, size, scale);
#endif

#ifdef PTK_DEBUG
        if (info.backend == WindowInfo::Backend::Hardware)
            PTK_WARN("Could not create hardware context for platform: No hardware context available for platform.");
#endif

        return MakeRasterContext(window, size, scale);
    }

    bool IsContextAvailable(ContextBackendType type)
    {
        auto available = {ContextBackendType::Raster,
#ifdef PTK_OPENGL
                          ContextBackendType::GL
#endif
        };

        for (auto it{available.begin()}; it != available.end(); ++it)
            if (*it == type)
                return true;

        return false;
    }

} // namespace pTK::Platform::ContextFactoryImpl
