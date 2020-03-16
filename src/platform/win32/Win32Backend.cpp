//
//  platform/win32/Win32Window.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "ptk/platform/win32/Win32Backend.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/events/WindowEvent.hpp"

// C++ Headers
#include <iostream>

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
    Win32Backend::Win32Backend(Window *window, const std::string& name, const Vec2u& size, Backend backend)
        : WindowBackend(backend),
            m_parentWindow{window}, m_handle{}
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

        if(!RegisterClassExW(&wcx))
            throw WindowError("Window Registration Failed!");

        DWORD style = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
        Size wSize{static_cast<Size::value_type>(size.x), static_cast<Size::value_type>(size.y)};
        Size adjSize{calcAdjustedWindowSize(wSize, style)};
        m_handle = CreateWindowExW(0, wcx.lpszClassName, get_utf16(name).c_str(), style, CW_USEDEFAULT, CW_USEDEFAULT,
                                   adjSize.width, adjSize.height, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
        if (!m_handle)
            throw WindowError("Failed to create window!");

        rasterCanvas = new Win32RasterContext(wSize);
        SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG_PTR)m_parentWindow);

        ::ShowWindow(m_handle, SW_SHOW);
        ::UpdateWindow(m_handle);
    }

    void Win32Backend::setPosHint(const Point& pos)
    {
        // TODO
    }

    void Win32Backend::pollEvents()
    {
        PTK_WARN("pollEvents start");
        MSG Msg;
        WaitMessage();
        if(GetMessageW(&Msg, nullptr, 0, 0) > 0 ){
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        PTK_WARN("pollEvents end");
    }

    void Win32Backend::beginPaint()
    {
        ps = PAINTSTRUCT();
        HDC hdc{BeginPaint(m_handle, &ps)};
    }

    void Win32Backend::endPaint()
    {
        EndPaint(m_handle, &ps);
    }

    void Win32Backend::swapbuffers()
    {
        rasterCanvas->swapbuffers(m_handle);
    }

    void Win32Backend::resize(const Size& size)
    {
        rasterCanvas->resize(m_parentWindow->getContentSize());
    }

    void Win32Backend::setLimits(const Size& min, const Size& max)
    {
        // TODO
    }

    void Win32Backend::close()
    {
        // TODO
        m_parentWindow->postEvent(new Event{Event::Category::Window, Event::Type::WindowClose});
    }

    void Win32Backend::show()
    {
        // TODO
        m_parentWindow->forceDrawAll();
    }

    void Win32Backend::hide()
    {
        // TODO
    }

    ContextBase *Win32Backend::getContext() const
    {
        return rasterCanvas;
    }

    Vec2f Win32Backend::getDPIScale() const
    {
        return {1.0f, 1.0f};
    }

    LRESULT Win32Backend::wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
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
                {
                    KeyEvent evt{Event::Type::KeyPressed, static_cast<int32>(wParam)};
                    window->sendEvent(&evt);
                }
                break;
            case WM_KEYUP:
                {
                    KeyEvent evt{Event::Type::KeyReleased, static_cast<int32>(wParam)};
                    window->sendEvent(&evt);
                }
                break;
            case WM_MOUSEMOVE:
                {
                    MotionEvent evt{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                    window->sendEvent(&evt);
                }
                break;
            case WM_LBUTTONDOWN:
                handleMouseClick(window, Event::Type::MouseButtonPressed, Mouse::Button::Left, lParam);
                break;
            case WM_LBUTTONUP:
                handleMouseClick(window, Event::Type::MouseButtonReleased, Mouse::Button::Left, lParam);
                break;
            case WM_SIZE:
                window->postEvent(new ResizeEvent(LOWORD(lParam), HIWORD(lParam)));
                //window->handleEvents();
                break;
            default:
                return DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        return 0;
    }

    void Win32Backend::handleMouseClick(Window *window, Event::Type type, Mouse::Button btn, LPARAM lParam)
    {
        Point::value_type xpos = static_cast<Point::value_type>(GET_X_LPARAM(lParam));
        Point::value_type ypos = static_cast<Point::value_type>(GET_Y_LPARAM(lParam));
        ButtonEvent evt{type, btn, xpos, ypos};
        window->sendEvent(&evt);
    }

    Size Win32Backend::calcAdjustedWindowSize(const Size& from, DWORD style) const
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