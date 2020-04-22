//
//  platform/win/WinRasterContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-11.
//

#ifndef PTK_PlATFORM_WIN_WINRASTERCONTEXT_HPP
#define PTK_PlATFORM_WIN_WINRASTERCONTEXT_HPP

// Local Headers
#include "ptk/core/ContextBase.hpp"

// Windows Headers
#include <windows.h>

// Skia Headers
#include "src/core/SkAutoMalloc.h"

namespace pTK
{
    /** WinRasterContext class implementation.

        Context for a software based Windows backend.
        All drawings will be done using the CPU.
    */
    class WinRasterContext : public ContextBase
    {
    public:
        /** Constructs Win32RasterCanvas with default values.

            @return    default initialized Win32RasterCanvas
        */
        WinRasterContext(HWND hwnd, const Size& size);

        /** Win32RasterCanvas for GLCanvas.

        */
        virtual ~WinRasterContext() = default;

        /** Function for resizing the context.

            @param width   New width of canvas
            @param width   New height of canvas
        */
        void resize(const Size& size) override;

        /** Function for retrieving the SkCanvas of the context.

            @return    SkCanvas property
        */
        SkCanvas* skCanvas() const override;

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        SkSurface* skSurface() const override;

        /** Function for swapping the buffers.

        */
        void swapBuffers() override;

    private:
        HWND m_hwnd;
        SkAutoMalloc  m_surfaceMemory;
        sk_sp<SkSurface> m_surface;
    };
}

#endif // PTK_PlATFORM_WIN_WINRASTERCONTEXT_HPP
