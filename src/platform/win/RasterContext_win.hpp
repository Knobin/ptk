//
//  platform/win/RasterContext_win.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-07.
//

#ifndef PTK_PlATFORM_WIN_RASTERCONTEXT_HPP
#define PTK_PlATFORM_WIN_RASTERCONTEXT_HPP

// pTK Headers
#include "ptk/platform/common/RasterContext.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include <include/core/SkImageInfo.h>
PTK_DISABLE_WARN_END()

// Windows Headers
#define NOMINMAX
#include <windows.h>

namespace pTK::Platform
{
    /** RasterContext_win class implementation.

        Raster Context for the Windows backend.
    */
    class PTK_API RasterContext_win : public RasterContext
    {
    public:
        /** Deleted constructor for RasterContext_win

        */
        RasterContext_win() = delete;

        /** Constructs RasterContext_win with default values.

            @param hwnd   HWND
            @return       default initialized RasterContext_win
        */
        RasterContext_win(HWND hwnd, const Size& size);

        /** Destructor for RasterContext_win.

        */
        virtual ~RasterContext_win();

        /** Function for resizing.

            @param size     new size
            @return         pointer to pixel storage, nullptr if error
        */
        void* onResize(const Size& size) override;

        /** Function to swap the buffers after drawing.

        */
        void swapBuffers() override;

    public:
        // std::size_t size{0};
        // void* pixels{nullptr};

    private:
        BITMAPINFO* bmpInfo{nullptr};
        HWND m_hwnd;
    };
} // namespace pTK::Platform

#endif // PTK_PlATFORM_WIN_RASTERCONTEXT_HPP
