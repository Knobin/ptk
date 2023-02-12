//
//  platform/win/ContextFactory.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-12.
//

// Local Headers
#include "RasterPolicy_win.hpp"
#include "WindowHandle_win.hpp"

// Include GL backend.
#ifdef PTK_OPENGL
    #include "GLContext_win.hpp"
#endif

// pTK Headers
#include "ptk/Log.hpp"
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

    static HWND GetHWND(Window* window)
    {
        // Could use reinterpret_cast here, but the handle should always be for macOS when
        // this function gets called.
        if (auto platformHandle = reinterpret_cast<WindowHandle_win*>(window->handle()))
            return platformHandle->handle();

        PTK_ASSERT(nullptr, "Window ptr is not of type WindowHandle_win");
        return nullptr;
    }

    std::unique_ptr<ContextBase> MakeRasterContext(Window* window, const Size& size, const Vec2f& scale)
    {
        // Software backend is always available.
        return std::make_unique<RasterContext<RasterPolicy_win>>(ScaleSize(size, scale),
                                                                 RasterPolicy_win{GetHWND(window)});
    }

    std::unique_ptr<ContextBase> MakeGLContext(Window* window, const Size& size, const Vec2f& scale)
    {
#ifdef PTK_OPENGL
        return std::make_unique<GLContext_win>(GetHWND(window), ScaleSize(size, scale));
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
