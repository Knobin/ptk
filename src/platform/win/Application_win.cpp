//
//  platform/win/Platform_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

// Local Headers
#include "Application_win.hpp"
#include "ptk/core/Exception.hpp"
#include "MainWindow_win.hpp"

namespace pTK
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

    Application_win::Application_win()
        : ApplicationBase()
    {
        ::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        if (!registerClass())
            throw PlatformError("Failed to register class!");

        PTK_INFO("Windows platform initialized");
    }

    Application_win::~Application_win()
    {
        ::UnregisterClassW(L"PTK", GetModuleHandleW(nullptr));

        PTK_INFO("Application_win destroyed");
    }

    int Application_win::messageLoop()
    {
        // Multiple windows is currently unsupported.
        // It is guaranteed to be at least one window here.
        Window *window{windows().at(1)};
        PaintEvent evt{Point{0, 0}, window->getSize()};
        window->sendEvent(&evt);
        window->show();

        while (!window->shouldClose())
        {
            window->handleEvents();
            window->waitEvents();
        }

        window->hide();

        return 0;
    }

    void Application_win::close()
    {
        // TODO: close app.
    }

    std::wstring Application_win::stringToUTF16(const std::string& str)
    {
        if (str.empty())
            return std::wstring();
        int sz{::MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), nullptr, 0)};
        std::wstring res(sz, 0);
        ::MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), &res[0], sz);
        return res;
    }
}
