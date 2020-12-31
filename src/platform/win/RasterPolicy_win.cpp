//
//  platform/win/RasterPolicy_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-07.
//

// Local Headers
#include "RasterPolicy_win.hpp"
#include "ptk/Log.hpp"

namespace pTK
{
    RasterPolicy_win::RasterPolicy_win(HWND hwnd)
        : m_hwnd{hwnd}
    {
        PTK_INFO("Initialized RasterPolicy_win");
    }

    RasterPolicy_win::~RasterPolicy_win()
    {
        std::free(bmpInfo);
        PTK_INFO("Destroyed RasterPolicy_win");
    }

    bool RasterPolicy_win::resize(const Size& nSize)
    {
        std::free(bmpInfo);
        bmpInfo = nullptr;

        const std::size_t width{static_cast<std::size_t>(nSize.width)};
        const std::size_t height{static_cast<std::size_t>(nSize.height)};
        const std::size_t bmpSize = sizeof(BITMAPINFOHEADER) + width * height * sizeof(uint32_t);

        void *bmpPtr{std::malloc(bmpSize)};
        if (!bmpPtr)
            return false;

        bmpInfo = static_cast<BITMAPINFO*>(bmpPtr);
        ZeroMemory(bmpInfo, sizeof(BITMAPINFO));
        bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo->bmiHeader.biWidth = static_cast<long>(width);
        bmpInfo->bmiHeader.biHeight = -static_cast<long>(height);
        bmpInfo->bmiHeader.biPlanes = 1;
        bmpInfo->bmiHeader.biBitCount = 32;
        bmpInfo->bmiHeader.biCompression = BI_RGB;
        pixels = bmpInfo->bmiColors;
        size = sizeof(uint32_t) * width * height;

        wSize = nSize;
        PTK_INFO("Sized RasterPolicy_win to {}x{}", width, height);
        return true;
    }

    void RasterPolicy_win::swapBuffers() const
    {
        HRGN hrgn{CreateRectRgn(0,0,0,0)};
        GetWindowRgn(m_hwnd, hrgn);
        HDC dc{GetDCEx(m_hwnd, hrgn, DCX_PARENTCLIP)};
        StretchDIBits(dc, 0, 0, wSize.width, wSize.height, 0, 0, wSize.width, wSize.height, bmpInfo->bmiColors, bmpInfo,
                      DIB_RGB_COLORS, SRCCOPY);
        DeleteObject(hrgn);
        ReleaseDC(m_hwnd, dc);
    }
}
