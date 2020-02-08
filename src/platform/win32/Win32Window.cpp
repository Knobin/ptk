//
//  platform/win32/Win32Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "ptk/platform/win32/Win32Window.hpp"

// C++ Headers
#include <iostream>
#include <exception>

static std::wstring get_utf16(const std::string& str)
{
    if (str.empty()) return std::wstring();
    int sz = MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), 0, 0);
    std::wstring res(sz, 0);
    MultiByteToWideChar(CP_ACP, 0, &str[0], (int)str.size(), &res[0], sz);
    return res;
}

namespace pTK
{
    Window::Window(const std::string& name, const Vec2u& size)
        : BaseWindow(name, size),
            m_handle{}
    {
        WNDCLASSEXW wcx{};
        wcx.cbSize = sizeof(wcx);
        wcx.style = CS_HREDRAW | CS_VREDRAW;
        wcx.hInstance = GetModuleHandleW(nullptr);;
        wcx.lpfnWndProc = wndPro;
        wcx.lpszClassName = L"PTK";
        wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcx.hCursor = ::LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW));

        if(!RegisterClassExW(&wcx)) {
            std::cerr << "Window Registration Failed!" << std::endl;
            throw std::exception();
        }

        DWORD style = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
        m_handle = CreateWindowExW(0, wcx.lpszClassName, get_utf16(name).c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT,
                                      size.x, size.y, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
        if(!m_handle){
            std::cerr << "Error: Failure to create window!" << std::endl;
            throw std::exception();
        }

        SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG_PTR)this);

        ::ShowWindow(m_handle, SW_SHOW);
        ::UpdateWindow(m_handle);
    }

    void Window::pollEvents()
    {
        MSG Msg;
        while(GetMessageW(&Msg, NULL, 0, 0) > 0 ){
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }

    LRESULT Window::wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
        switch(msg)
        {
            case WM_CREATE:
                break;
            case WM_CLOSE:
                DestroyWindow(hwnd);
                window->postEvent(new Event{Event::Category::Window, Event::Type::WindowClose});
                break;
            case WM_DESTROY:
                PostQuitMessage(0);
                break;
            case WM_MOVE:
                break;
            case WM_PAINT:
                window->postEvent(new Event{Event::Category::Window, Event::Type::WindowDraw});
                break;
            default:
                return DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        return 0;
    }
}