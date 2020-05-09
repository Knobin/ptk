//
//  platform/win/WinBackend.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "WinBackend.hpp"
#include "WinPlatform.hpp"
#include "WinRasterContext.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/events/KeyCodes.hpp"
#include "ptk/events/WindowEvent.hpp"

// Include OpenGL backend if HW Acceleration is enabled.
#ifdef PTK_HW_ACCELERATION
#include "WinGLContext.hpp"
#endif // PTK_HW_ACCELERATION

// Windows Headers
#include <windowsx.h>

// C++ Headers
#include <cmath>

namespace pTK
{
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

    static std::map<byte, int32> s_keyMap{initKeyCodes()};

    static int32 translateKeyCode(byte code)
    {
        auto it{s_keyMap.find(code)};
        if (it != s_keyMap.end())
            return it->second;

        return PTK_KEY_UNKNOWN;
    }

    static Size calcAdjustedReverseWindowSize(const Size& from, DWORD style, uint dpi)
    {
        RECT adjustedSize{};
        ::SetRectEmpty(&adjustedSize);
        ::AdjustWindowRectExForDpi(&adjustedSize, style, FALSE, 0, dpi);

        return {from.width - (adjustedSize.right - adjustedSize.left),
                from.height - (adjustedSize.bottom - adjustedSize.top)};
    }

    static Size calcAdjustedWindowSize(const Size& from, DWORD style, uint dpi)
    {
        RECT adjustedSize{};
        adjustedSize.top = 0;
        adjustedSize.bottom = from.height;
        adjustedSize.left = 0;
        adjustedSize.right = from.width;
        ::AdjustWindowRectExForDpi (&adjustedSize, style, FALSE, 0, dpi);
        return {adjustedSize.right - adjustedSize.left, adjustedSize.bottom - adjustedSize.top};
    }

    static std::unique_ptr<ContextBase> createWin32Context(BackendType type, HWND hwnd,
                                                           const Size& size)
    {
#ifdef PTK_HW_ACCELERATION
        if (type == BackendType::HARDWARE)
            return std::make_unique<WinGLContext>(hwnd, size);
#endif // PTK_HW_ACCELERATION

        // Software backend is always available.
        return std::make_unique<WinRasterContext>(hwnd, size);
    }

    static Size scaleSize(const Size& size, const Vec2f& scale)
    {
        Size newSize{};
        newSize.width = static_cast<Size::value_type>(std::ceil(size.width * scale.x));
        newSize.height = static_cast<Size::value_type>(std::ceil(size.height * scale.y));
        return newSize;
    }

    struct WinBackendData
    {
        Window *window;
        Vec2f scale;
        Size size;
        Point pos;
        DWORD style;
    };

    WinBackend::WinBackend(Window *window, const std::string& name, const Size& size, BackendType backend)
        : WindowBackend(backend),
            m_parentWindow{window}, m_handle{}, m_context{nullptr}
    {
        // High DPI
        HDC screen{GetDC(nullptr)};
        float dpiX{static_cast<float>(::GetDeviceCaps(screen, LOGPIXELSX))};
        float dpiY{static_cast<float>(::GetDeviceCaps(screen, LOGPIXELSY))};
#ifdef PTK_DEBUG
    if (dpiX != dpiY)
    {
        PTK_WARN("DPI for x and y is not the same!");
    }
#endif
        ::ReleaseDC(nullptr, screen);
        PTK_INFO("Windows Window DPI {}x{}", dpiX, dpiY);
        Vec2f scale{dpiX / 96.0f, dpiY / 96.0f};
        PTK_INFO("Windows Window Scaling {0:0.2f}x{1:0.2f}", scale.x, scale.y);

        Size wSize{scaleSize(size, scale)};
        DWORD style{WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX};
        Size adjSize{calcAdjustedWindowSize(wSize, style, dpiX)};
        m_handle = ::CreateWindowExW(0, L"PTK", WinPlatform::stringToUTF16(name).c_str(), style,
                                     CW_USEDEFAULT, CW_USEDEFAULT, adjSize.width, adjSize.height,
                                     nullptr, nullptr, ::GetModuleHandleW(nullptr), nullptr);
        if (!m_handle)
            throw WindowError("Failed to create window!");
        PTK_INFO("Created WinWindow: {}x{}", wSize.width, wSize.height);

        m_context = createWin32Context(backend, m_handle, wSize);
        m_data = std::make_unique<WinBackendData>(WinBackendData{window, scale, wSize, {}, style});
        SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG_PTR)m_data.get());

        ::ShowWindow(m_handle, SW_SHOW);
        ::UpdateWindow(m_handle);
    }

    WinBackend::~WinBackend()
    {

    }

    void WinBackend::setPosHint(const Point& pos)
    {
        RECT rect{pos.x, pos.y, pos.x, pos.y};
        ::SetWindowPos(m_handle, nullptr, rect.left, rect.top, 0, 0,
                     SWP_NOACTIVATE | SWP_NOZORDER | SWP_NOSIZE);
    }

    void WinBackend::pollEvents()
    {
        MSG Msg;
        if (::GetMessageW(&Msg, nullptr, 0, 0) > 0)
        {
            ::TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
    }

    void WinBackend::beginPaint()
    {
        ps = PAINTSTRUCT();
        HDC hdc{BeginPaint(m_handle, &ps)};
    }

    void WinBackend::endPaint()
    {
        ::EndPaint(m_handle, &ps);
    }

    void WinBackend::setTitle(const std::string& name)
    {
        ::SetWindowTextW(m_handle, WinPlatform::stringToUTF16(name).c_str());
    }

    void WinBackend::setIcon(int32 width, int32 height, byte* pixels)
    {
        // DIB information.
        BITMAPV5HEADER bmInfo{};
        ZeroMemory(&bmInfo, sizeof(bmInfo));
        bmInfo.bV5Size = sizeof(bmInfo);
        bmInfo.bV5Width = width;
        bmInfo.bV5Height = -height;
        bmInfo.bV5Planes = 1;
        bmInfo.bV5BitCount = 32;
        bmInfo.bV5Compression = BI_BITFIELDS;
        bmInfo.bV5RedMask = 0x00FF0000;
        bmInfo.bV5GreenMask = 0x0000FF00;
        bmInfo.bV5BlueMask = 0x000000FF;
        bmInfo.bV5AlphaMask = 0xFF000000;

        // Create the DIB.
        HDC dc{::GetDC(nullptr)};
        byte* target{nullptr};
        HBITMAP color{::CreateDIBSection(dc, reinterpret_cast<BITMAPINFO*>(&bmInfo), DIB_RGB_COLORS,
                                         reinterpret_cast<void**>(&target), nullptr, 0)};
        ::ReleaseDC(nullptr, dc);
        PTK_ASSERT(color, "Failed to create HBITMAP");

        // Transform pixels from RGBA to BGRA.
        for (int i = 0; i < width * height; i++)
        {
            target[(i * 4)] = pixels[(i * 4) + 2];
            target[(i * 4) + 1] = pixels[(i * 4) + 1];
            target[(i * 4) + 2] = pixels[(i * 4) + 0];
            target[(i * 4) + 3] = pixels[(i * 4) + 3];
        }

        // Create the mask.
        HBITMAP mask{::CreateBitmap(width, height, 1, 1, nullptr)};
        PTK_ASSERT(mask, "Failed to create HBITMAP");

        // Finally, create the icon.
        ICONINFO iconInfo{true, 0, 0, mask, color};
        HICON hIcon{::CreateIconIndirect(&iconInfo)};
        PTK_ASSERT(hIcon, "Failed to create the icon");

        ::DeleteObject(color);
        ::DeleteObject(mask);

        // Apply icon.
        SendMessage(m_handle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(m_handle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));

        // Ensure that the application icon is changed.
        SendMessage(::GetWindow(m_handle, GW_OWNER), WM_SETICON, ICON_SMALL,
                    reinterpret_cast<LPARAM>(hIcon));
        SendMessage(::GetWindow(m_handle, GW_OWNER), WM_SETICON, ICON_BIG,
                    reinterpret_cast<LPARAM>(hIcon));
    }

    void WinBackend::notifyEvent()
    {
        // Event has been pushed from a different thread.
        // Signal the window to redraw.
        ::RedrawWindow(m_handle, nullptr, nullptr, RDW_INTERNALPAINT | RDW_UPDATENOW);
    }

    DWORD WinBackend::getWindowStyle() const
    {
        return m_data->style;
    }

    void WinBackend::swapBuffers()
    {
        m_context->swapBuffers();
    }

    void WinBackend::resize(const Size& size)
    {
        // Apply the DPI scaling.
        const Size scaledSize{scaleSize(size, m_data->scale)};

        if (scaledSize != m_data->size)
        {
            // Adjust the size depending on the window frame.
            const Size adjSize{calcAdjustedWindowSize(scaledSize, m_data->style, m_data->scale.x * 96.0f)};

            // Get the window size and position.
            RECT rect{};
            ::GetWindowRect(m_handle, &rect);

            m_data->pos = Point{rect.left, rect.top};
            m_data->size = scaledSize;

            // Apply the new size to the context and window.
            if (scaledSize != m_context->getSize())
                m_context->resize(scaledSize);
            ::MoveWindow(m_handle, m_data->pos.x, m_data->pos.y, adjSize.width, adjSize.height, FALSE);
        }
    }

    void WinBackend::close()
    {
        ::DestroyWindow(m_handle);
    }

    void WinBackend::show()
    {
        ::ShowWindow(m_handle, SW_SHOW);
        m_parentWindow->forceDrawAll();
    }

    void WinBackend::hide()
    {
        ::ShowWindow(m_handle, SW_HIDE);
    }

    bool WinBackend::isHidden() const
    {
        return static_cast<bool>(::IsWindowVisible(m_handle));
    }

    ContextBase* WinBackend::getContext() const
    {
        return m_context.get();
    }

    Vec2f WinBackend::getDPIScale() const
    {
        return m_data->scale;
    }

    Point WinBackend::getWinPos() const
    {
        return m_data->pos;
    }

    Size WinBackend::getWinSize() const
    {
        return m_data->size;
    }

    void WinBackend::setLimits(const Size&, const Size&)
    {
        RECT rect{};
        ::GetWindowRect(m_handle, &rect);
        ::MoveWindow(m_handle, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                     TRUE);
    }

    void WinBackend::minimize()
    {
        ::ShowWindow(m_handle, SW_MINIMIZE);
    }

    bool WinBackend::isMinimized() const
    {
        return static_cast<bool>(IsMinimized(m_handle));
    }

    void WinBackend::restore()
    {
        ::ShowWindow(m_handle, SW_RESTORE);
    }

    bool WinBackend::isFocused() const
    {
        return (m_handle == ::GetFocus());
    }

    void WinBackend::setScaleHint(const Vec2f& scale)
    {
        if (m_data->scale != scale)
        {
            m_data->scale = scale;
            resize(m_parentWindow->getSize());
        }
    }

    static void handleMouseClick(WinBackendData* data, Event::Type type, Mouse::Button btn,
                                 LPARAM lParam)
    {
        if (data->window)
        {
            const Point pos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            ButtonEvent evt{type,
                            btn,
                            {static_cast<Point::value_type>(pos.x * (1 / data->scale.x)),
                             static_cast<Point::value_type>(pos.y * (1 / data->scale.y))}};
            data->window->sendEvent(&evt);
        }
    }

    LRESULT WinBackend::wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        WinBackendData* data{
            reinterpret_cast<WinBackendData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))};
        Window* window{(data) ? data->window : nullptr};
        switch (msg)
        {
            case WM_CREATE:
                break;
            case WM_CLOSE:
                DestroyWindow(hwnd);
                break;
            case WM_DESTROY:
            {
                Event evt{Event::Category::Window, Event::Type::WindowClose};
                window->handleEvents(); // Handle all events before sending close event.
                window->sendEvent(&evt);
                ::PostQuitMessage(0);
                break;
            }
            case WM_SETFOCUS:
            {
                Event evt{Event::Category::Window, Event::Type::WindowFocus};
                window->sendEvent(&evt);
                break;
            }
            case WM_KILLFOCUS:
            {
                Event evt{Event::Category::Window, Event::Type::WindowLostFocus};
                window->sendEvent(&evt);
                break;
            }
            case WM_MOVE:
            {
                MoveEvent evt{{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)}};
                data->pos = evt.pos;
                window->sendEvent(&evt);
                break;
            }
            case WM_PAINT:
            {
                if (window->visible())
                    window->forceDrawAll();
                break;
            }
            case WM_KEYDOWN:
            {
                KeyEvent evt{KeyEvent::Pressed, translateKeyCode(static_cast<byte>(wParam))};
                window->sendEvent(&evt);
                break;
            }
            case WM_KEYUP:
            {
                KeyEvent evt{KeyEvent::Released, translateKeyCode(static_cast<byte>(wParam))};
                window->sendEvent(&evt);
                break;
            }
            case WM_MOUSEMOVE:
            {
                if (window)
                {
                    const Point pos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                    MotionEvent evt{{static_cast<Point::value_type>(pos.x * (1 / data->scale.x)),
                                     static_cast<Point::value_type>(pos.y * (1 / data->scale.y))}};
                    window->sendEvent(&evt);
                }
                break;
            }
            case WM_LBUTTONDOWN:
                handleMouseClick(data, Event::Type::MouseButtonPressed, Mouse::Button::Left,
                                 lParam);
                break;
            case WM_LBUTTONUP:
                handleMouseClick(data, Event::Type::MouseButtonReleased, Mouse::Button::Left,
                                 lParam);
                break;
            case WM_SIZING:
            {
                RECT* rect = reinterpret_cast<RECT*>(lParam);
                const Size size = {static_cast<Size::value_type>(rect->right - rect->left),
                                   static_cast<Size::value_type>(rect->bottom - rect->top)};
                ResizeEvent evt{scaleSize(calcAdjustedReverseWindowSize(size, data->style, data->scale.x * 96.0f), Vec2f{1.0f / data->scale.x, 1.0f / data->scale.y})};
                window->sendEvent(&evt);
                window->forceDrawAll();
                break;
            }
            case WM_SIZE:
            {
                if (window && window->getBackend())
                {
                    if (wParam == SIZE_MINIMIZED)
                    {
                        Event evt{Event::Category::Window, Event::Type::WindowMinimize};
                        window->sendEvent(&evt);
                    }
                    else if (wParam == SIZE_RESTORED)
                    {
                        Event evt{Event::Category::Window, Event::Type::WindowRestore};
                        window->sendEvent(&evt);
                    }
                    else
                    {
                        const Size size{static_cast<Size::value_type>(LOWORD(lParam)),
                                           static_cast<Size::value_type>(HIWORD(lParam))};
                        ResizeEvent evt{scaleSize(size, Vec2f{1.0f / data->scale.x, 1.0f / data->scale.y})};
                        window->sendEvent(&evt);
                        window->forceDrawAll();
                    }
                }
                break;
            }
            case WM_GETMINMAXINFO:
            {
                if (window)
                {
                    LPMINMAXINFO lpMMI{reinterpret_cast<LPMINMAXINFO>(lParam)};
                    const Size minSize{window->getMinSize()};
                    WinBackend* backend{static_cast<WinBackend*>(window->getBackend())};
                    const Size adjMinSize{calcAdjustedWindowSize(scaleSize(minSize, data->scale),
                                                                 backend->getWindowStyle(), data->scale.x * 96.0f)};
                    lpMMI->ptMinTrackSize.x = adjMinSize.width;
                    lpMMI->ptMinTrackSize.y = adjMinSize.height;
                    const Size maxSize{window->getMaxSize()};
                    if (maxSize != Size::Max)
                    {
                        const Size adjMaxSize{calcAdjustedWindowSize(
                            scaleSize(maxSize, data->scale), data->style, data->scale.x * 96.0f)};
                        lpMMI->ptMaxTrackSize.x = adjMaxSize.width;
                        lpMMI->ptMaxTrackSize.y = adjMaxSize.height;
                    }
                    else
                    {
                        lpMMI->ptMaxTrackSize.x = ::GetSystemMetrics(SM_CXMAXTRACK);
                        lpMMI->ptMaxTrackSize.y = ::GetSystemMetrics(SM_CYMAXTRACK);
                    }
                }
                break;
            }
            case WM_DPICHANGED:
            {
                // Change Window position.
                RECT* rect = reinterpret_cast<RECT*>(lParam);
                MoveEvent mEvt{{static_cast<Point::value_type>(rect->left),
                                   static_cast<Point::value_type>(rect->top)}};
                window->sendEvent(&mEvt);

                // Scale Window.
                const uint32 dpiX = static_cast<uint32>(GET_X_LPARAM(wParam));
                const uint32 dpiY = static_cast<uint32>(GET_Y_LPARAM(wParam));
                const Vec2f scale{dpiX / 96.0f, dpiY / 96.0f};
                PTK_INFO("DPI CHANGED {0}x{1} SCALING {2:0.2f}x{3:0.2f}", dpiX, dpiY, scale.x, scale.y);
                ScaleEvent sEvt{scale};
                window->sendEvent(&sEvt);
                break;
            }
            default:
                return ::DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        return 0;
    }
} // namespace pTK