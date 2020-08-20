//
//  platform/win/Platform_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

// Local Headers
#include "Platform_win.hpp"
#include "ptk/core/Exception.hpp"
#include "MainWindow_win.hpp"

namespace pTK::Platform_win
{
    static bool registerClass()
    {
        WNDCLASSEXW wcx{};
        wcx.cbSize = sizeof(wcx);
        wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcx.hInstance = GetModuleHandleW(nullptr);
        wcx.lpfnWndProc = MainWindow_win::wndPro;
        wcx.cbClsExtra = 0;
        wcx.cbWndExtra = 0;
        wcx.lpszMenuName = nullptr;
        wcx.lpszClassName = L"PTK";
        wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcx.hCursor = ::LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW));

        return ::RegisterClassExW(&wcx) != 0;
    }

    void init()
    {
        ::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        if (!registerClass())
            throw PlatformError("Failed to register class!");

        PTK_INFO("Windows platform initialized");
    }

    void destroy()
    {
        ::UnregisterClassW(L"PTK", GetModuleHandleW(nullptr));

        PTK_INFO("Windows platform destroyed");
    }

    std::wstring stringToUTF16(const std::string& str)
    {
        if (str.empty())
            return std::wstring();
        int sz{::MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), nullptr, 0)};
        std::wstring res(sz, 0);
        ::MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), &res[0], sz);
        return res;
    }
}
