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

// Windows Headers
#include <windowsx.h>

static bool render = true;

static std::wstring get_utf16(const std::string& str)
{
    if (str.empty())
        return std::wstring();
    int sz = MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
    std::wstring res(sz, 0);
    MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), &res[0], sz);
    return res;
}

namespace pTK
{
    Window::Window(const std::string& name, const Vec2u& size)
        : WindowBase(name, size),
            m_handle{}
    {
        WNDCLASSEXW wcx{};
        wcx.cbSize = sizeof(wcx);
        wcx.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        wcx.hInstance = GetModuleHandleW(nullptr);;
        wcx.lpfnWndProc = wndPro;
        wcx.cbClsExtra = 0;
        wcx.cbWndExtra = 0;
        wcx.lpszMenuName = nullptr;
        wcx.lpszClassName = L"PTK";
        wcx.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
        wcx.hCursor = ::LoadCursorW(nullptr, reinterpret_cast<LPCWSTR>(IDC_ARROW));

        if(!RegisterClassExW(&wcx)) {
            std::cerr << "Window Registration Failed!" << std::endl;
            throw std::exception();
        }


        DWORD style = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
        Size wSize{static_cast<Size::value_type>(size.x), static_cast<Size::value_type>(size.y)};
        Size adjSize{calcAdjustedWindowSize(wSize, style)};
        m_handle = CreateWindowExW(0, wcx.lpszClassName, get_utf16(name).c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT,
                                   adjSize.width, adjSize.height, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
        if (!m_handle)
        {
            std::cerr << "Error: Failure to create window!" << std::endl;
            throw std::exception();
        }

        rasterCanvas = new Win32RasterContext(wSize);
        SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG_PTR)this);

        ::ShowWindow(m_handle, SW_SHOW);
        ::UpdateWindow(m_handle);
    }

    void Window::forceDrawAll()
    {
        PAINTSTRUCT ps;
        HDC hdc{BeginPaint(m_handle, &ps)};
        SkCanvas* canvas = rasterCanvas->skCanvas();
        PTK_INFO("FORCEDRAWALL");
        Color color = getBackground();
        canvas->clear(SkColorSetARGB(255, color.r, color.g, color.b));
        // Apply monitor scale.
        SkMatrix matrix;
        Vec2f scale{getDPIScale()};
        matrix.setScale(scale.x, scale.y);
        canvas->setMatrix(matrix);
        for (iterator it = begin(); it != end(); it++)
            (*it)->onDraw(canvas);

        canvas->flush();
        rasterCanvas->swapbuffers(m_handle);
        EndPaint(m_handle, &ps);
    }

    void Window::pollEvents()
    {
        MSG Msg;
        WaitMessage();
        if(GetMessageW(&Msg, nullptr, 0, 0) > 0 ){
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
            case WM_KEYDOWN:
                window->sendEvent(new KeyEvent(Event::Type::KeyPressed, wParam));
                break;
            case WM_KEYUP:
                window->sendEvent(new KeyEvent(Event::Type::KeyReleased, wParam));
                break;
            case WM_MOUSEMOVE:
                window->sendEvent(new MotionEvent(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)));
                break;
            default:
                return DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        return 0;
    }

    Size Window::calcAdjustedWindowSize(const Size& from, DWORD style) const
    {
        RECT adjustedSize{};
        adjustedSize.top = 0;
        adjustedSize.bottom = from.height;
        adjustedSize.left = 0;
        adjustedSize.right = from.width;
        AdjustWindowRectEx(&adjustedSize, style, FALSE, 0);
        Size::value_type width = adjustedSize.right - adjustedSize.left;
        Size::value_type height = adjustedSize.bottom - adjustedSize.top;
        return {width, height};
    }
}