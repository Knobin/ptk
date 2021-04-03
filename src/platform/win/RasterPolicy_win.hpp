//
//  platform/win/RasterPolicy_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-07.
//

#ifndef PTK_PlATFORM_WIN_RASTERPOLICY_HPP
#define PTK_PlATFORM_WIN_RASTERPOLICY_HPP

// pTK Headers
#include "ptk/util/Size.hpp"
#include "ptk/Core.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include <include/core/SkImageInfo.h>
PTK_DISABLE_WARN_END()

// Windows Headers
#include <windows.h>

namespace pTK
{
    /** RasterPolicy_win class implementation.

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
    class RasterPolicy_win
    {
    public:
        /** Deleted constructor for RasterPolicy_win

        */
        RasterPolicy_win() = delete;

        /** Constructs RasterPolicy_win with default values.

            @param hwnd   HWND
            @return       default initialized RasterPolicy_win
        */
        explicit RasterPolicy_win(HWND hwnd);

        /** Destructor for RasterPolicy_win.

        */
        ~RasterPolicy_win();

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
        void *pixels{nullptr};
        const SkColorType colorType{kN32_SkColorType};

    private:
        BITMAPINFO *bmpInfo{nullptr};
        HWND m_hwnd;
        Size wSize{};
    };
}

#endif // PTK_PlATFORM_WIN_RASTERPOLICY_HPP
