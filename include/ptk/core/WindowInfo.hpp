//
//  core/WindowInfo.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-12-31.
//

#ifndef PTK_CORE_WINDOWINFO_HPP
#define PTK_CORE_WINDOWINFO_HPP

namespace pTK
{
    struct WindowInfo
    {
        /** Backend enum class implementation

            Specifies which backend that should be used.
        */
        enum class BackendType : byte
        {
            SOFTWARE = 1,
            HARDWARE
        };

        enum class Visibility : byte
        {
            Windowed = 0, // Default
            Hidden
            // For now only visible and hidden states are available.
            // TODO: Implement the following Minimized, Maximized and FullScreen.
        };
        
        BackendType backend{BackendType::HARDWARE};
        Point position{-1, -1};
        Visibility visibility{Visibility::Windowed};
    };
}

#endif // PTK_CORE_WINDOWINFO_HPP
