//
//  platform/base/ContextFactory.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

#ifndef PTK_PLATFORM_BASE_CONTEXTFACTORY_HPP
#define PTK_PLATFORM_BASE_CONTEXTFACTORY_HPP

// pTK Headers
#include "ptk/core/ContextBase.hpp"
#include "ptk/core/WindowInfo.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Vec2.hpp"

// C++ Headers
#include <memory>

namespace pTK
{
    // Forward declarations.
    class Window;

    namespace Platform
    {
        /** ContextFactory class implementation

            Specifies the context creation functions that are available to all platforms.

            The user can call any of these independent of platform, if the backend is
            not supported, the function will return nullptr.

            The platform must implement these in the pTK::Platform namespace for the linker:
                - std::unique_ptr<ContextBase> MakeContext(Window*, const Size&, const Vec2f&, const WindowInfo&);
                - std::unique_ptr<ContextBase> MakeRasterContext(Window*, const Size&, const Vec2f&);
                - IsContextAvailable(ContextBackendType type);

            Optional functions to implement depending on what is available is:
                - If PTK_OPENGL is defined, you must implement:
                    * std::unique_ptr<ContextBase> MakeGLContext(Window*, const Size&, const Vec2f&);
                - If PTK_METAL is defined, you must implement:
                    - std::unique_ptr<ContextBase> MakeMetalContext(Window*, const Size&, const Vec2f&);
        */
        class ContextFactory
        {
        public:
            /** Function for creating a context based on the info.

                Creates a hardware context if the flag is defined.
                If no hardware context is available on the platform, a raster
                context will be created even if a hardware context is specified
                in the flags.

                @return     Best Context based on provided info
            */
            static std::unique_ptr<ContextBase> Make(Window* window, const Size& size, const Vec2f& scale,
                                                     const WindowInfo& info);

            /** Function for creating a raster context for software rendering.

                @return     Raster Context
            */
            static std::unique_ptr<ContextBase> MakeRaster(Window* window, const Size& size, const Vec2f& scale);

            /** Function for creating a OpenGL context for hardware rendering.

                Note: will return nullptr if backend is not available.

                @return     OpenGL Context
            */
            static std::unique_ptr<ContextBase> MakeGL(Window* window, const Size& size, const Vec2f& scale);

            /** Function for creating a Metal context for hardware rendering.

                Note: will return nullptr if backend is not available.

                @return     Metal Context
            */
            static std::unique_ptr<ContextBase> MakeMetal(Window* window, const Size& size, const Vec2f& scale);

            /** Function for checking if a specific backend type is available on the platform.

                @return     status
            */
            static bool IsAvailable(ContextBackendType type);
        };
    } // namespace Platform
} // namespace pTK

#endif // PTK_PLATFORM_BASE_CONTEXTFACTORY_HPP
