//
//  platform/win/RasterContext_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-07.
//

// Local Headers
#include "RasterContext_win.hpp"
#include "../../Log.hpp"

namespace pTK::Platform
{
    RasterContext_win::RasterContext_win(HWND hwnd, const Size& size)
        : RasterContext(kN32_SkColorType, size),
          m_hwnd{hwnd}
    {
        resize(size);
        PTK_INFO("Initialized RasterContext_win");
    }

    RasterContext_win::~RasterContext_win()
    {
        std::free(bmpInfo);
        PTK_INFO("Destroyed RasterContext_win");
    }

    void* RasterContext_win::onResize(const Size& nSize)
    {
        std::free(bmpInfo);
        bmpInfo = nullptr;

        const std::size_t width{static_cast<std::size_t>(nSize.width)};
        const std::size_t height{static_cast<std::size_t>(nSize.height)};
        const std::size_t bmpSize = sizeof(BITMAPINFOHEADER) + width * height * sizeof(uint32_t);

        void* bmpPtr{std::malloc(bmpSize)};
        if (!bmpPtr)
            return nullptr;

        bmpInfo = static_cast<BITMAPINFO*>(bmpPtr);
        ZeroMemory(bmpInfo, sizeof(BITMAPINFO));
        bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo->bmiHeader.biWidth = static_cast<long>(width);
        bmpInfo->bmiHeader.biHeight = -static_cast<long>(height);
        bmpInfo->bmiHeader.biPlanes = 1;
        bmpInfo->bmiHeader.biBitCount = 32;
        bmpInfo->bmiHeader.biCompression = BI_RGB;

        // size = sizeof(uint32_t) * width * height;
        // wSize = nSize;

        PTK_INFO("Sized RasterContext_win to {}x{}", width, height);
        return bmpInfo->bmiColors;
    }

    void RasterContext_win::swapBuffers()
    {
        const auto size{getSize()};

        HRGN hrgn{CreateRectRgn(0, 0, 0, 0)};
        GetWindowRgn(m_hwnd, hrgn);
        HDC dc{GetDCEx(m_hwnd, hrgn, DCX_PARENTCLIP)};
        StretchDIBits(dc, 0, 0, size.width, size.height, 0, 0, size.width, size.height, bmpInfo->bmiColors, bmpInfo,
                      DIB_RGB_COLORS, SRCCOPY);
        DeleteObject(hrgn);
        ReleaseDC(m_hwnd, dc);
    }
} // namespace pTK::Platform
