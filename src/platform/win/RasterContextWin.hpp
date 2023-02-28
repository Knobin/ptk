//
//  platform/win/RasterContextWin.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-07.
//

#ifndef PTK_PlATFORM_WIN_RASTERCONTEXT_HPP
#define PTK_PlATFORM_WIN_RASTERCONTEXT_HPP

// pTK Headers
#include "ptk/platform/RasterContext.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include <include/core/SkImageInfo.h>
PTK_DISABLE_WARN_END()

// Windows Headers
#define NOMINMAX
#include <windows.h>

namespace pTK::Platform
{
    /** RasterContextWin class implementation.

        Raster Context for the Windows backend.
    */
    class PTK_API RasterContextWin : public RasterContext
    {
    public:
        /** Deleted constructor for RasterContextWin

        */
        RasterContextWin() = delete;

        /** Constructs RasterContextWin with hwnd and size.

            @param hwnd     win32 window handle
            @param size     size of the context
            @return         initialized RasterContextWin
        */
        RasterContextWin(HWND hwnd, const Size& size);

        /** Destructor for RasterContextWin.

        */
        virtual ~RasterContextWin();

        /** Function for resizing.

            @param size     new size
            @return         pointer to pixel storage, nullptr if error
        */
        void* onResize(const Size& size) override;

        /** Function to swap the buffers after drawing.

        */
        void swapBuffers() override;

    private:
        BITMAPINFO* m_bmpInfo{nullptr};
        HWND m_hwnd;
    };
} // namespace pTK::Platform

#endif // PTK_PlATFORM_WIN_RASTERCONTEXT_HPP
