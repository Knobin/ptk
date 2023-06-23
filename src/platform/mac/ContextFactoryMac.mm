//
//  src/platform/mac/ContextFactoryMac.mm
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

// Local Headers
#include "../../include/Log.hpp"
#include "../../include/Assert.hpp"
#include "RasterContextMac.hpp"
#include "WindowHandleMac.hpp"

// Include Metal backend.
#ifdef PTK_METAL
    #include "MetalContextMac.hpp"
#endif

// pTK Headers
#include "ptk/Window.hpp"
#include "ptk/core/Defines.hpp"
#include "ptk/platform/ContextFactory.hpp"
#include "ptk/platform/RasterContext.hpp"

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
        if (auto platformHandle = dynamic_cast<WindowHandleMac*>(window->platformHandle()))
            return static_cast<NSWindow*>(platformHandle->nswindow());

        PTK_ASSERT(nullptr, "Window ptr is not of type WindowHandle_mac");
        return nullptr;
    }

    std::unique_ptr<ContextBase> MakeRasterContext(Window* window, const Size& size, const Vec2f& scale)
    {
        // Software backend is always available.
        // Altough, it is slow on macOS and should be avoided.
        PTK_WARN("Software rendering on macOS is slow and should not be used");
        return std::make_unique<RasterContextMac>(GetNSWindow(window), ScaleSize(size, scale));
    }

    std::unique_ptr<ContextBase> MakeMetalContext(Window* window, const Size& size, const Vec2f& scale)
    {
#ifdef PTK_METAL
        auto view = static_cast<void*>([GetNSWindow(window) contentView]);
        return std::make_unique<MetalContextMac>(view, ScaleSize(size, scale), scale);
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

} // namespace pTK::Platform::ContextFactoryImpl
