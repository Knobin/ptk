//
//  platform/winapi/Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "ptk/platform/winapi/Window.hpp"

// C++ Headers
#include <iostream>
#include <stdexcept>

namespace pTK
{
    namespace WinAPI
    {
        Window::Window()
            : m_handle{}
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
            m_handle = CreateWindowExW(0, wcx.lpszClassName, L"Window", style, CW_USEDEFAULT, CW_USEDEFAULT,
                                          960, 540, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
            if(!m_handle){
                std::cerr << "Error: Failure to create window!" << std::endl;
                throw std::exception();
            }

            ::ShowWindow(m_handle, SW_SHOW);
            ::UpdateWindow(m_handle);

            MSG Msg;
            while(GetMessageW(&Msg, NULL, 0, 0) > 0 ){
                TranslateMessage(&Msg);
                DispatchMessage(&Msg);
            }
        }

        Window::~Window()
        {

        }

        LRESULT Window::wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
        {
            switch(msg)
            {
                case WM_CREATE:
                    break;
                case WM_CLOSE:
                    DestroyWindow(hwnd);
                    break;
                case WM_DESTROY:
                    PostQuitMessage(0);
                    break;
                case WM_MOVE:
                    break;
                case WM_PAINT:
                    break;
                default:
                    return DefWindowProcW(hwnd, msg, wParam, lParam);
            }

            return 0;
        }
    }
}