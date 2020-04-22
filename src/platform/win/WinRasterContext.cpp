//
//  platform/win/WinRasterCanvas.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-11.
//

// Local Headers
#include "ptk/platform/win/WinRasterContext.hpp"
#include "ptk/core/Exception.hpp"

namespace pTK
{
    WinRasterContext::WinRasterContext(HWND hwnd, const Size& size)
        : ContextBase(size), m_hwnd{hwnd}
    {
        resize(size);
    }

    void WinRasterContext::resize(const Size& size)
    {
        SkColorType m_colorType{kN32_SkColorType};

        uint w = size.width;
        uint h = size.height;
        m_surface.reset();
        const size_t bmpSize = sizeof(BITMAPINFOHEADER) + w * h * sizeof(uint32_t);
        m_surfaceMemory.reset(bmpSize);
        BITMAPINFO* bmpInfo = reinterpret_cast<BITMAPINFO*>(m_surfaceMemory.get());
        ZeroMemory(bmpInfo, sizeof(BITMAPINFO));
        bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo->bmiHeader.biWidth = w;
        bmpInfo->bmiHeader.biHeight = -h;
        bmpInfo->bmiHeader.biPlanes = 1;
        bmpInfo->bmiHeader.biBitCount = 32;
        bmpInfo->bmiHeader.biCompression = BI_RGB;
        void* pixels = bmpInfo->bmiColors;

        SkImageInfo info = SkImageInfo::Make(w, h, m_colorType, kPremul_SkAlphaType,nullptr);
        m_surface = SkSurface::MakeRasterDirect(info, pixels, sizeof(uint32_t) * w);
        if (!m_surface)
            throw ContextError("Failed to create Raster Canvas for Windows Window");

        PTK_INFO("Created WinRasterCanvas: {}x{}", size.width, size.height);
        setSize(size);
    }

    SkCanvas* WinRasterContext::skCanvas() const
    {
        return m_surface->getCanvas();
    }

    SkSurface* WinRasterContext::skSurface() const
    {
        return m_surface.get();
    }

    void WinRasterContext::swapBuffers()
    {
        BITMAPINFO *bmpInfo = reinterpret_cast<BITMAPINFO*>(m_surfaceMemory.get());
        HRGN hrgn = CreateRectRgn(0,0,0,0);
        GetWindowRgn(m_hwnd, hrgn);
        HDC dc = GetDCEx(m_hwnd, hrgn, DCX_PARENTCLIP);
        const Size size = getSize();
        StretchDIBits(dc, 0, 0, size.width, size.height, 0, 0, size.width, size.height, bmpInfo->bmiColors, bmpInfo,
                      DIB_RGB_COLORS, SRCCOPY);
        DeleteObject(hrgn);
        ReleaseDC(m_hwnd, dc);
    }
}