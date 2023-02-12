//
//  src/platform/mac/ContextFactory_mac.mm
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// Local Headers
#include "RasterPolicy_mac.hpp"
#include "WindowHandle_mac.hpp"

// Include Metal backend.
#ifdef PTK_METAL
    #include "MetalContext_mac.hpp"
#endif

// pTK Headers
#include "ptk/Log.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Defines.hpp"
#include "ptk/platform/base/ContextFactory.hpp"
#include "ptk/platform/common/RasterContext.hpp"

// macOS Headers
#include <Cocoa/Cocoa.h>

namespace pTK::Platform::ContextFactoryImpl
{
    static Size ScaleSize(const Size& size, const Vec2f& scale)
    {
#ifdef PTK_DEBUG
        if (scale.x != scale.y)
            PTK_WARN("Context scale is not the same, x: {}, y: {}", static_cast<double>(scale.x),
                     static_cast<double>(scale.y));
#endif
        return {static_cast<Size::value_type>(size.width * scale.x),
                static_cast<Size::value_type>(size.height * scale.y)};
    }

    static NSWindow* GetNSWindow(Window* window)
    {
        // Could use reinterpret_cast here, but the handle should always be for macOS when
        // this function gets called.
        auto handle = static_cast<WindowHandle_mac*>(window->handle());
        return static_cast<NSWindow*>(handle->handle());
    }

    std::unique_ptr<ContextBase> MakeRasterContext(Window* window, const Size& size, const Vec2f& scale)
    {
        // Software backend is always available.
        // Altough, it is slow on macOS and should be avoided.
        PTK_WARN("Software rendering on macOS is slow and should not be used");
        RasterPolicy_mac policy(GetNSWindow(window));
        return std::make_unique<RasterContext<RasterPolicy_mac>>(ScaleSize(size, scale), policy);
    }

    std::unique_ptr<ContextBase> MakeMetalContext(Window* window, const Size& size, const Vec2f& scale)
    {
#ifdef PTK_METAL
        auto view = static_cast<void*>([GetNSWindow(window) contentView]);
        return std::make_unique<MetalContext_mac>(view, ScaleSize(size, scale), scale);
#endif
        return nullptr;
    }

    std::unique_ptr<ContextBase> MakeContext(Window* window, const Size& size, const Vec2f& scale,
                                             const WindowInfo& info)
    {
#ifdef PTK_METAL
        if (info.backend == WindowInfo::Backend::Hardware)
            return MakeMetalContext(window, size, scale);
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
#ifdef PTK_METAL
                          ContextBackendType::Metal
#endif
        };

        for (auto it{available.begin()}; it != available.end(); ++it)
            if (*it == type)
                return true;

        return false;
    }

} // namespace pTK::Platform
