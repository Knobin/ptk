//
//  platform/mac/RasterPolicy_mac.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PlATFORM_MAC_RASTERPOLICY_HPP
#define PTK_PlATFORM_MAC_RASTERPOLICY_HPP

// Local Headers
#include "ptk/util/Size.hpp"

// Skia Headers
#include "include/core/SkImageInfo.h"

// macOS Headers
#include <Cocoa/Cocoa.h>

namespace pTK
{
    class RasterPolicy_mac
    {
    public:
        RasterPolicy_mac() = delete;
        RasterPolicy_mac(NSWindow *window);
        ~RasterPolicy_mac();
        
        bool resize(const Size& newSize);
        void swapBuffers() const;
        
    public:
        std::size_t size{0};
        void *pixels{nullptr};
        const SkColorType colorType{kN32_SkColorType};
        
    private:
        NSWindow *m_window;
        CGContextRef m_gc;
    };
}

#endif // PTK_PlATFORM_MAC_RASTERPOLICY_HPP
