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
        if (m_bmpInfo != nullptr)
            std::free(m_bmpInfo);
        PTK_INFO("Destroyed RasterContext_win");
    }

    void* RasterContext_win::onResize(const Size& nSize)
    {
        std::free(m_bmpInfo);
        m_bmpInfo = nullptr;

        const std::size_t width{static_cast<std::size_t>(nSize.width)};
        const std::size_t height{static_cast<std::size_t>(nSize.height)};
        const std::size_t bmpSize = sizeof(BITMAPINFOHEADER) + width * height * sizeof(uint32_t);

        void* bmpPtr{std::malloc(bmpSize)};
        if (!bmpPtr)
            return nullptr;

        m_bmpInfo = static_cast<BITMAPINFO*>(bmpPtr);
        ZeroMemory(m_bmpInfo, sizeof(BITMAPINFO));
        m_bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        m_bmpInfo->bmiHeader.biWidth = static_cast<long>(width);
        m_bmpInfo->bmiHeader.biHeight = -static_cast<long>(height);
        m_bmpInfo->bmiHeader.biPlanes = 1;
        m_bmpInfo->bmiHeader.biBitCount = 32;
        m_bmpInfo->bmiHeader.biCompression = BI_RGB;

        PTK_INFO("Sized RasterContext_win to {}x{}", width, height);
        return m_bmpInfo->bmiColors;
    }

    void RasterContext_win::swapBuffers()
    {
        const auto size{getSize()};
        const auto width{static_cast<int>(size.width)};
        const auto height{static_cast<int>(size.height)};

        HRGN hrgn{CreateRectRgn(0, 0, 0, 0)};
        GetWindowRgn(m_hwnd, hrgn);
        HDC dc{GetDCEx(m_hwnd, hrgn, DCX_PARENTCLIP)};
        StretchDIBits(dc, 0, 0, width, height, 0, 0, width, height, m_bmpInfo->bmiColors, m_bmpInfo, DIB_RGB_COLORS,
                      SRCCOPY);
        DeleteObject(hrgn);
        ReleaseDC(m_hwnd, dc);
    }
} // namespace pTK::Platform
