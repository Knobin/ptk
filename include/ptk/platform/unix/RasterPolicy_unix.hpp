//
//  platform/unix/RasterPolicy_unix.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-17.
//

#ifndef PTK_PLATFORM_UNIX_RASTERPOLICY_HPP
#define PTK_PLATFORM_UNIX_RASTERPOLICY_HPP

// Local Headers
#include "ptk/platform/unix/x11.hpp"

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/util/Size.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include <include/core/SkImageInfo.h>
PTK_DISABLE_WARN_END()

namespace pTK
{
    /** RasterPolicy_unix class implementation.

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
    class PTK_API RasterPolicy_unix
    {
    public:
        /** Deleted constructor for RasterPolicy_unix

        */
        RasterPolicy_unix() = delete;

        /** Constructs RasterPolicy_unix with default values.

            @param hwnd   HWND
            @return       default initialized RasterPolicy_unix
        */
        RasterPolicy_unix(::Window* window, XVisualInfo info);

        /** Destructor for RasterPolicy_unix.

        */
        ~RasterPolicy_unix();

        /** Function for resizing.

            @param nSize    new size
            @return         true if resized, otherwise false
        */
        bool resize(const Size& nSize);

        /** Function to swap the buffers after drawing.

        */
        void swapBuffers() const;

    public:
        std::size_t size{0};
        void* pixels{nullptr};
        const SkColorType colorType{kBGRA_8888_SkColorType};

    private:
        ::Window* m_window;
        XVisualInfo m_info;
        XImage* m_image;
        GC m_gc;
        Size wSize;
    };
} // namespace pTK

#endif // PTK_PLATFORM_UNIX_RASTERPOLICY_HPP
