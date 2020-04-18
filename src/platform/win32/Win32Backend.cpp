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

namespace pTK
{
    static std::wstring get_utf16(const std::string& str)
    {
        if (str.empty())
            return std::wstring();
        int sz = MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), nullptr, 0);
        std::wstring res(sz, 0);
        MultiByteToWideChar(CP_ACP, 0, &str[0], static_cast<int>(str.size()), &res[0], sz);
        return res;
    }

    static Size calcAdjustedWindowSize(const Size& from, DWORD style)
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

    static Size calcScaling(const Size& size, const Vec2f& scale)
    {
        return {static_cast<Size::value_type>(size.width * scale.x),
                     static_cast<Size::value_type>(size.height * scale.y)};
    }

    Win32Backend::Win32Backend(Window *window, const std::string& name, const Size& size, Backend backend)
        : WindowBackend(backend),
            m_parentWindow{window}, m_handle{},
            m_style{WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX},
            m_scale{1.0f, 1.0f}
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

        // High DPI
        HDC screen = GetDC(nullptr);
        auto dpiX = static_cast<float>(GetDeviceCaps(screen, LOGPIXELSX));
        auto dpiY = static_cast<float>(GetDeviceCaps(screen, LOGPIXELSY));
        ReleaseDC(nullptr, screen);
        PTK_INFO("Win32 Window DPI {}x{}", dpiX, dpiY);
        m_scale.x = dpiX / 96.0f;
        m_scale.y = dpiY / 96.0f;
        PTK_INFO("Win32 Window Scaling {0:0.2f}x{1:0.2f}", m_scale.x, m_scale.y);

        //Size wSize{calcScaling(size, m_scale)};
        Size wSize{size};
        Size adjSize{calcAdjustedWindowSize(wSize, m_style)};
        m_handle = CreateWindowExW(0, wcx.lpszClassName, get_utf16(name).c_str(), m_style, CW_USEDEFAULT, CW_USEDEFAULT,
                                   adjSize.width, adjSize.height, nullptr, nullptr, GetModuleHandleW(nullptr), nullptr);
        if (!m_handle)
            throw WindowError("Failed to create window!");
        PTK_INFO("Created Win32Window: {}x{}", size.width, size.height);

        rasterCanvas = new Win32RasterContext(wSize);
        SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG_PTR)m_parentWindow);

        ::ShowWindow(m_handle, SW_SHOW);
        ::UpdateWindow(m_handle);
    }

    void Win32Backend::setPosHint(const Point& pos)
    {
        RECT rect{pos.x, pos.y, pos.x, pos.y};
        SetWindowPos(m_handle, nullptr, rect.left, rect.top, 0, 0,
                SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    }

    void Win32Backend::pollEvents()
    {
        MSG Msg;
        if(GetMessageW(&Msg, nullptr, 0, 0) > 0 ){
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
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

    DWORD Win32Backend::getWindowStyle() const
    {
        return m_style;
    }

    void Win32Backend::swapBuffers()
    {
        rasterCanvas->swapBuffers(m_handle);
    }

    void Win32Backend::resize(const Size& size)
    {
        rasterCanvas->resize(size);
    }

    void Win32Backend::close()
    {
        m_parentWindow->postEvent(new Event{Event::Category::Window, Event::Type::WindowClose});
    }

    void Win32Backend::show()
    {
        ShowWindow(m_handle, SW_SHOW);
        m_parentWindow->forceDrawAll();
    }

    void Win32Backend::hide()
    {
        ShowWindow(m_handle, SW_HIDE);
    }

    ContextBase *Win32Backend::getContext() const
    {
        return rasterCanvas;
    }

    Vec2f Win32Backend::getDPIScale() const
    {
        return {1.0f, 1.0f};
    }

    void Win32Backend::setLimits(const Size&, const Size&)
    {
        RECT rect{};
        GetWindowRect(m_handle, &rect);
        MoveWindow(m_handle,
                rect.left, rect.top,
                rect.right - rect.left,
                rect.bottom - rect.top, TRUE);
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
                if (window->visible())
                {
                    window->postEvent(new Event{Event::Category::Window, Event::Type::WindowDraw});
                    window->handleEvents();
                }
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
            case WM_SIZING:
            {
                RECT rect{};
                GetClientRect(hwnd, &rect);
                int width = rect.right - rect.left;
                int height = rect.bottom - rect.top;
                window->postEvent(new ResizeEvent(width, height));
            }
                break;
            case WM_SIZE:
                window->postEvent(new ResizeEvent(LOWORD(lParam), HIWORD(lParam)));
                break;
            case WM_GETMINMAXINFO:
            {
                if (window)
                {
                    LPMINMAXINFO lpMMI = (LPMINMAXINFO)lParam;
                    const Size minSize{window->getMinSize()};
                    Win32Backend* backend{static_cast<Win32Backend*>(window->getBackend())};
                    const Size adjMinSize{calcAdjustedWindowSize(minSize, backend->getWindowStyle())};
                    lpMMI->ptMinTrackSize.x = adjMinSize.width;
                    lpMMI->ptMinTrackSize.y = adjMinSize.height;
                    const Size maxSize{window->getMaxSize()};
                    if (maxSize != Size::Max)
                    {
                        const Vec2f scale{window->getDPIScale()};
                        const Size adjMaxSize{calcAdjustedWindowSize(maxSize, backend->getWindowStyle())};
                        // const Size adjMaxSize{calcAdjustedWindowSize(calcScaling(maxSize, scale), backend->getWindowStyle())};
                        lpMMI->ptMaxTrackSize.x = adjMaxSize.width;
                        lpMMI->ptMaxTrackSize.y = adjMaxSize.height;
                    } else
                    {
                        lpMMI->ptMaxTrackSize.x = GetSystemMetrics(SM_CXMAXTRACK);
                        lpMMI->ptMaxTrackSize.y = GetSystemMetrics(SM_CYMAXTRACK);
                    }
                }

            }
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
}