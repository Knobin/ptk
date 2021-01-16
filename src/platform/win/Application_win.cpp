//
//  platform/win/Platform_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

// Local Headers
#include "Application_win.hpp"
#include "MainWindow_win.hpp"

// pTK Headers
#include "ptk/core/Exception.hpp"
#include "ptk/Application.hpp"

namespace pTK
{
    static bool RegisterWndClass()
    {
        WNDCLASSEXW wcx{};
        wcx.cbSize = sizeof(wcx);
        wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcx.hInstance = ::GetModuleHandleW(nullptr);
        wcx.lpfnWndProc = MainWindow_win::WndPro;
        wcx.cbClsExtra = 0;
        wcx.cbWndExtra = 0;
        wcx.lpszMenuName = nullptr;
        wcx.lpszClassName = L"PTK";
        wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcx.hCursor = ::LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW));

        return ::RegisterClassExW(&wcx) != 0;
    }

    // Internal map for keeping MainWindow_win pointers.
    // By doing this we can skip the dynamic_cast step in pollEvents().
    // It is a vector because it should be faster to traverse (no random pointers).
    // Since Windows also can be removed when in the loop, the iterator can be invalidated...
    // Might look into this later since Windows and the App is supposed to run on the same
    // thread, it works and is acceptable behaviour for now.
    static std::vector<std::pair<int32, MainWindow_win*>> s_windows{};
    static std::vector<std::pair<int32, MainWindow_win*>>::iterator s_windowIter{};
    static bool s_erased{false};

    Application_win::Application_win()
        : ApplicationBase()
    {
        ::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        if (!RegisterWndClass())
            throw PlatformError("Failed to register class!");

        PTK_INFO("Initialized Application_win");
    }

    Application_win::~Application_win()
    {
        ::UnregisterClassW(L"PTK", GetModuleHandleW(nullptr));

        PTK_INFO("Destroyed Application_win");
    }

    void Application_win::pollEvents()
    {
        MSG msg{};
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            s_windowIter = s_windows.begin();
            for (auto it = s_windowIter; it != s_windows.end();)
            {
                MainWindow_win *window = it->second;
                if (HACCEL accel = window->accelTable())
                {
                    if (TranslateAccelerator(window->handle(), accel, &msg))
                    {
                        ++it;
                        continue;
                    }
                }

                ::TranslateMessage(&msg);
                DispatchMessage(&msg);

                if (s_erased)
                {
                    s_erased = false;
                    it = s_windowIter;
                }
                else
                    ++it;
            }
        }
    }

    void Application_win::waitEvents()
    {
        WaitMessage();
        pollEvents();
    }

    void Application_win::waitEventsTimeout(uint ms)
    {
        MsgWaitForMultipleObjects(0, nullptr, FALSE, static_cast<DWORD>(ms), QS_ALLEVENTS);
        pollEvents();
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

    void Application_win::onWindowAdd(const std::pair<int32, Window*> item)
    {
        if (auto wWin = dynamic_cast<MainWindow_win*>(item.second->getBackend()))
            s_windows.emplace_back(item.first, wWin);
    }

    void Application_win::onWindowRemove(const std::pair<int32, Window*> item)
    {
        auto it = std::find_if(s_windows.begin(), s_windows.end(), [item](const auto& pair){
            return pair.first == item.first;
        });
        if (it != s_windows.end())
        {
            s_windowIter = s_windows.erase(it);
            s_erased = true;
        }

    }
}
