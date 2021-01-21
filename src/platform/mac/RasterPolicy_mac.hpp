//
//  platform/mac/RasterPolicy_mac.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

#ifndef PTK_PlATFORM_MAC_RASTERPOLICY_HPP
#define PTK_PlATFORM_MAC_RASTERPOLICY_HPP

// pTK Headers
#include "ptk/util/Size.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkImageInfo.h"
PTK_DISABLE_WARN_END()

// macOS Headers
#include <Cocoa/Cocoa.h>

namespace pTK
{
    /** RasterPolicy_mac class implementation.

        This is a policy for the RasterContext class.
        It needs these public functions:
            - resize
            - swapBuffers
        and these public variables:
            - size
            - pixels
            - colorType
        For more information see the RasterContext header file.
    */
    class RasterPolicy_mac
    {
    public:
        /** Deleted constructor for RasterPolicy_mac

         */
        RasterPolicy_mac() = delete;
        
        /** Constructs RasterPolicy_mac with default values.

            @param window   NSWindow pointer
            @return         default initialized RasterPolicy_mac
        */
        RasterPolicy_mac(NSWindow *window);
        
        /** Deconstructor for RasterPolicy_mac.

        */
        ~RasterPolicy_mac();
        
        /** Function for resizing the context.

            @param size   New width and height of canvas
        */
        bool resize(const Size& newSize);
        
        /** Function for swapping the buffers.

        */
        void swapBuffers() const;
        
    public:
        std::size_t size{0}; // Size of the buffer
        void *pixels{nullptr}; // Pointer to buffer
        const SkColorType colorType{kN32_SkColorType};
        
    private:
        NSWindow *m_window;
        CGContextRef m_gc;
    };
}

#endif // PTK_PlATFORM_MAC_RASTERPOLICY_HPP
