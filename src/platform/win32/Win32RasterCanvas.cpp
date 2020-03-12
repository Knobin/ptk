//
//  platform/win32/Win32RasterCanvas.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-03-11.
//

// Local Headers
#include "ptk/platform/win32/Win32RasterCanvas.hpp"

namespace pTK
{
    Win32RasterCanvas::Win32RasterCanvas(const Size& size)
        : CanvasBase(size)
    {
        resize(size);
    }

    void Win32RasterCanvas::resize(const Size& size)
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
        bmpInfo->bmiHeader.biHeight = -h; // negative means top-down bitmap. Skia draws top-down.
        bmpInfo->bmiHeader.biPlanes = 1;
        bmpInfo->bmiHeader.biBitCount = 32;
        bmpInfo->bmiHeader.biCompression = BI_RGB;
        void* pixels = bmpInfo->bmiColors;

        SkImageInfo info = SkImageInfo::Make(w, h, m_colorType, kPremul_SkAlphaType,nullptr);
        m_surface = SkSurface::MakeRasterDirect(info, pixels, sizeof(uint32_t) * w);
        PTK_ASSERT(m_surface, "Failed to create Raster Canvas for Win32");

        PTK_INFO("Created Win32RasterCanvas: {}x{}", size.width, size.height);
        setSize(size);
    }

    SkCanvas* Win32RasterCanvas::skCanvas() const
    {
        return m_surface->getCanvas();
    }

    SkSurface* Win32RasterCanvas::skSurface() const
    {
        return m_surface.get();
    }

    void Win32RasterCanvas::swapbuffers(HWND m_handle)
    {
        BITMAPINFO *bmpInfo = reinterpret_cast<BITMAPINFO*>(m_surfaceMemory.get());
        HRGN hrgn = CreateRectRgn(0,0,0,0);
        GetWindowRgn(m_handle, hrgn);
        HDC dc = GetDCEx(m_handle, hrgn, DCX_PARENTCLIP);
        const Size size = getSize();
        StretchDIBits(dc, 0, 0, size.width, size.height, 0, 0, size.width, size.height, bmpInfo->bmiColors, bmpInfo,
                      DIB_RGB_COLORS, SRCCOPY);
        DeleteObject(hrgn);
        ReleaseDC(m_handle, dc);
        PTK_INFO("Swapped Buffers");
    }
}