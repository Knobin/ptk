//
//  platform/win32/Win32RasterContext.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-11.
//

#ifndef PTK_PlATFORM_WIN32_WIN32RASTERCONTEXT_HPP
#define PTK_PlATFORM_WIN32_WIN32RASTERCONTEXT_HPP

// Local Headers
#include "ptk/core/ContextBase.hpp"

// Windows Headers
#include <windows.h>

// Skia Headers
#include "src/core/SkAutoMalloc.h"

namespace pTK
{
    class Win32RasterContext : public ContextBase
    {
    public:
        /** Constructs Win32RasterCanvas with default values.

            @return    default initialized Win32RasterCanvas
        */
        Win32RasterContext(const Size& size);

        /** Win32RasterCanvas for GLCanvas.

        */
        virtual ~Win32RasterContext() = default;

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

        void swapbuffers(HWND hwnd);

    private:
        SkAutoMalloc  m_surfaceMemory;
        sk_sp<SkSurface> m_surface;
    };
}

#endif //PTK_PlATFORM_WIN32_WIN32RASTERCONTEXT_HPP
