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
#include "ptk/events/KeyCodes.hpp"
#include "ptk/platform/win32/Win32RasterContext.hpp"
#include "ptk/platform/win32/Win32GLContext.hpp"

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

    static std::map<byte, int32> initKeyCodes()
    {
        std::map<byte, int32> map{};
        map[0x00] = PTK_KEY_UNKNOWN;
        map[VK_SPACE] = PTK_KEY_SPACE; map[VK_ESCAPE] = PTK_KEY_ESCAPE;
        map[0x30] = PTK_KEY_0; map[0x31] = PTK_KEY_1; map[0x32] = PTK_KEY_2; map[0x33] = PTK_KEY_3;
        map[0x34] = PTK_KEY_4; map[0x35] = PTK_KEY_5; map[0x36] = PTK_KEY_6; map[0x37] = PTK_KEY_7;
        map[0x38] = PTK_KEY_8; map[0x39] = PTK_KEY_9;
        map[VK_NUMPAD0] = PTK_KEY_0; map[VK_NUMPAD1] = PTK_KEY_1; map[VK_NUMPAD2] = PTK_KEY_2;
        map[VK_NUMPAD3] = PTK_KEY_3; map[VK_NUMPAD4] = PTK_KEY_4; map[VK_NUMPAD5] = PTK_KEY_5;
        map[VK_NUMPAD6] = PTK_KEY_6; map[VK_NUMPAD7] = PTK_KEY_7; map[VK_NUMPAD8] = PTK_KEY_8;
        map[VK_NUMPAD9] = PTK_KEY_9;
        map[0x41] = PTK_KEY_A; map[0x42] = PTK_KEY_B; map[0x43] = PTK_KEY_C; map[0x44] = PTK_KEY_D;
        map[0x45] = PTK_KEY_E; map[0x46] = PTK_KEY_F; map[0x47] = PTK_KEY_G; map[0x48] = PTK_KEY_H;
        map[0x49] = PTK_KEY_I; map[0x4A] = PTK_KEY_J; map[0x4B] = PTK_KEY_K; map[0x4C] = PTK_KEY_L;
        map[0x4D] = PTK_KEY_M; map[0x4E] = PTK_KEY_N; map[0x4F] = PTK_KEY_O; map[0x50] = PTK_KEY_P;
        map[0x51] = PTK_KEY_Q; map[0x52] = PTK_KEY_R; map[0x53] = PTK_KEY_S; map[0x54] = PTK_KEY_T;
        map[0x55] = PTK_KEY_U; map[0x56] = PTK_KEY_V; map[0x57] = PTK_KEY_W; map[0x58] = PTK_KEY_X;
        map[0x59] = PTK_KEY_Y; map[0x5A] = PTK_KEY_Z;
        return map;
    }

    static int32 translateKeycode(const std::map<byte, int32>& map, byte keycode)
    {
        auto it{map.find(keycode)};
        if (it != map.end())
            return it->second;

        return PTK_KEY_UNKNOWN;
    }

    static std::map<byte, int32> s_keyMap{initKeyCodes()};

    std::unique_ptr<ContextBase> createWin32Context(BackendType type, HWND hwnd, const Size& size)
    {
        std::unique_ptr<ContextBase> context{nullptr};

        if (type == BackendType::SOFTWARE)
        {
            context = std::make_unique<Win32RasterContext>(hwnd, size);
        }
        else if (type == BackendType::HARDWARE)
        {
            context = std::make_unique<Win32GLContext>(hwnd, size);
        }
#ifdef PTK_DEBUG
        else
        {
            PTK_ASSERT(false, "Undefined backend type!");
        }
#endif

        return std::move(context);
    }

    Win32Backend::Win32Backend(Window *window, const std::string& name, const Size& size, BackendType backend)
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

        rasterCanvas = createWin32Context(backend, m_handle, wSize);
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
        rasterCanvas->swapBuffers();
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
        return rasterCanvas.get();
    }

    Vec2f Win32Backend::getDPIScale() const
    {
        return m_scale;
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
                // TODO: Should set the parent Window position property (getPosition should work correctly).
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
                    KeyEvent evt{Event::Type::KeyPressed, translateKeycode(s_keyMap, static_cast<byte>(wParam))};
                    window->sendEvent(&evt);
                }
                break;
            case WM_KEYUP:
                {
                    KeyEvent evt{Event::Type::KeyReleased, translateKeycode(s_keyMap, static_cast<byte>(wParam))};
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