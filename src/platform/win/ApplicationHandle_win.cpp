//
//  platform/win/Platform_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-04-26.
//

// Local Headers
#include "ApplicationHandle_win.hpp"
#include "WindowHandle_win.hpp"

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/core/Exception.hpp"

namespace pTK::Platform
{
    static bool RegisterWndClass()
    {
        WNDCLASSEXW wcx{};
        wcx.cbSize = sizeof(wcx);
        wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcx.hInstance = ::GetModuleHandleW(nullptr);
        wcx.lpfnWndProc = WindowHandle_win::WndPro;
        wcx.cbClsExtra = 0;
        wcx.cbWndExtra = 0;
        wcx.lpszMenuName = nullptr;
        wcx.lpszClassName = L"PTK";
        wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcx.hCursor = ::LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW));

        return ::RegisterClassExW(&wcx) != 0;
    }

    // Internal map for keeping WindowHandle_win pointers.
    // By doing this we can skip the dynamic_cast step in pollEvents().
    // It is a vector because it should be faster to traverse (no random pointers).
    // Since Windows also can be removed when in the loop, the iterator can be invalidated...
    // Might look into this later since Windows and the App is supposed to run on the same
    // thread, it works and is acceptable behaviour for now.
    static std::vector<std::pair<int32_t, WindowHandle_win*>> s_windows{};
    static std::vector<std::pair<int32_t, WindowHandle_win*>>::iterator s_windowIter{};
    static bool s_erased{false};

    ApplicationHandle_win::ApplicationHandle_win(ApplicationBase* base, std::string_view)
        : ApplicationHandle(base)
    {
        ::SetProcessDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

        if (!RegisterWndClass())
            throw PlatformError("Failed to register class!");

        PTK_INFO("Initialized ApplicationHandle_win");
    }

    ApplicationHandle_win::~ApplicationHandle_win()
    {
        ::UnregisterClassW(L"PTK", GetModuleHandleW(nullptr));

        PTK_INFO("Destroyed ApplicationHandle_win");
    }

    void ApplicationHandle_win::pollEvents()
    {
        MSG msg{};
        while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            s_windowIter = s_windows.begin();
            for (auto it = s_windowIter; it != s_windows.end();)
            {
                WindowHandle_win* window = it->second;
                if (HACCEL accel = window->accelTable())
                {
                    if (TranslateAccelerator(window->handle(), accel, &msg))
                    {
                        ++it;
                        continue;
                    }
                }

                ::TranslateMessage(&msg);
                if (msg.hwnd)
                    DispatchMessage(&msg);
                else if (msg.message == WM_QUIT)
                    if (Application::Get()->close())
                        ::PostQuitMessage(0);

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

    void ApplicationHandle_win::waitEvents()
    {
        WaitMessage();
        pollEvents();
    }

    void ApplicationHandle_win::waitEventsTimeout(uint32_t ms)
    {
        MsgWaitForMultipleObjects(0, nullptr, FALSE, static_cast<DWORD>(ms), QS_ALLEVENTS);
        pollEvents();
    }

    std::wstring ApplicationHandle_win::stringToUTF16(const std::string& str)
    {
        if (str.empty())
            return {};
        int sz{::MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), nullptr, 0)};
        std::wstring res(sz, 0);
        ::MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), &res[0], sz);
        return res;
    }

    void ApplicationHandle_win::onWindowAdd(int32_t key, Window* window)
    {
        if (auto handle = dynamic_cast<WindowHandle_win*>(window->handle()))
            s_windows.emplace_back(key, handle);
    }

    void ApplicationHandle_win::onWindowRemove(int32_t key, Window* window)
    {
        auto it = std::find_if(s_windows.begin(), s_windows.end(), [key, window](const auto& pair) {
            return pair.first == key || pair.second == static_cast<WindowHandle_win*>(window->handle());
        });
        if (it != s_windows.end())
        {
            s_windowIter = s_windows.erase(it);
            s_erased = true;
        }
    }
} // namespace pTK
