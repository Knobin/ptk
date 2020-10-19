//
//  platform/win/MainWindow_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "MainWindow_win.hpp"
#include "Application_win.hpp"
#include "../common/RasterContext.hpp"
#include "RasterPolicy_win.hpp"

// Include OpenGL backend if HW Acceleration is enabled.
#ifdef PTK_OPENGL
#include "GLContext_win.hpp"
#endif // PTK_OPENGL

// Windows Headers
#include <windowsx.h>
#include <Dwmapi.h>

// C++ Headers
#include <cmath>

namespace pTK
{
    static std::unique_ptr<ContextBase> createWin32Context([[maybe_unused]] BackendType type, HWND hwnd,
                                                           const Size& size)
    {
#ifdef PTK_OPENGL
        if (type == BackendType::HARDWARE)
            return std::make_unique<GLContext_win>(hwnd, size);
#endif // PTK_OPENGL

        // Software backend is always available.
        RasterPolicy_win policy{hwnd};
        return std::make_unique<RasterContext<RasterPolicy_win>>(size, policy);
    }

    ///////////////////////////////////////////////////////////////////////////////

    static std::map<byte, KeyCode> initKeyCodes() noexcept
    {
        std::map<byte, KeyCode> map{};
        map[VK_SPACE] = Key::Space; map[VK_ESCAPE] = Key::Escape;
        map[0x30] = Key::D0; map[0x31] = Key::D1; map[0x32] = Key::D2; map[0x33] = Key::D3;
        map[0x34] = Key::D4; map[0x35] = Key::D5; map[0x36] = Key::D6; map[0x37] = Key::D7;
        map[0x38] = Key::D8; map[0x39] = Key::D9;
        map[VK_NUMPAD0] = Key::D0; map[VK_NUMPAD1] = Key::D1; map[VK_NUMPAD2] = Key::D2;
        map[VK_NUMPAD3] = Key::D3; map[VK_NUMPAD4] = Key::D4; map[VK_NUMPAD5] = Key::D5;
        map[VK_NUMPAD6] = Key::D6; map[VK_NUMPAD7] = Key::D7; map[VK_NUMPAD8] = Key::D8;
        map[VK_NUMPAD9] = Key::D9;
        map[0x41] = Key::A; map[0x42] = Key::B; map[0x43] = Key::C; map[0x44] = Key::D;
        map[0x45] = Key::E; map[0x46] = Key::F; map[0x47] = Key::G; map[0x48] = Key::H;
        map[0x49] = Key::I; map[0x4A] = Key::J; map[0x4B] = Key::K; map[0x4C] = Key::L;
        map[0x4D] = Key::M; map[0x4E] = Key::N; map[0x4F] = Key::O; map[0x50] = Key::P;
        map[0x51] = Key::Q; map[0x52] = Key::R; map[0x53] = Key::S; map[0x54] = Key::T;
        map[0x55] = Key::U; map[0x56] = Key::V; map[0x57] = Key::W; map[0x58] = Key::X;
        map[0x59] = Key::Y; map[0x5A] = Key::Z;
        return map;
    }

    static const std::map<byte, KeyCode> s_keyMap{initKeyCodes()};

    static Key translateKeyCode(byte code)
    {
        std::map<byte, KeyCode>::const_iterator it{s_keyMap.find(code)};
        if (it != s_keyMap.cend())
            return it->second;

        return Key::Unknown;
    }

    ///////////////////////////////////////////////////////////////////////////////

    static Size calcAdjustedReverseWindowSize(const Size& from, DWORD style, float dpi)
    {
        RECT adjustedSize{};
        ::SetRectEmpty(&adjustedSize);
        ::AdjustWindowRectExForDpi(&adjustedSize, style, FALSE, 0, static_cast<UINT>(dpi));

        return {from.width - (adjustedSize.right - adjustedSize.left),
                from.height - (adjustedSize.bottom - adjustedSize.top)};
    }

    static Size calcAdjustedWindowSize(const Size& from, DWORD style, float dpi)
    {
        RECT adjustedSize{};
        adjustedSize.top = 0;
        adjustedSize.bottom = from.height;
        adjustedSize.left = 0;
        adjustedSize.right = from.width;
        ::AdjustWindowRectExForDpi (&adjustedSize, style, FALSE, 0, static_cast<UINT>(dpi));
        return {adjustedSize.right - adjustedSize.left, adjustedSize.bottom - adjustedSize.top};
    }

    static Size scaleSize(const Size& size, const Vec2f& scale) noexcept
    {
        Size newSize{};
        newSize.width = static_cast<Size::value_type>(std::ceil(size.width * scale.x));
        newSize.height = static_cast<Size::value_type>(std::ceil(size.height * scale.y));
        return newSize;
    }

    ///////////////////////////////////////////////////////////////////////////////

    struct WinBackendData
    {
        Window *window;
        Vec2f scale;
        Size size;
        Point pos;
        DWORD style;
        bool minimized;
        uint wait;
        bool ignoreSize;
    };

    ///////////////////////////////////////////////////////////////////////////////
    
    MainWindow_win::MainWindow_win(Window *window, const std::string& name, const Size& size, BackendType backend)
        : MainWindowBase(window, backend)
    {
        // High DPI
        HDC screen{GetDC(nullptr)};
        const float dpiX{static_cast<float>(::GetDeviceCaps(screen, LOGPIXELSX))};
        const float dpiY{static_cast<float>(::GetDeviceCaps(screen, LOGPIXELSY))};
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

        const Size wSize{scaleSize(size, scale)};
        constexpr DWORD style{WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX};
        const Size adjSize{calcAdjustedWindowSize(wSize, style, dpiX)};
        m_hwnd = ::CreateWindowExW(0, L"PTK", Application_win::stringToUTF16(name).c_str(), style,
                                     CW_USEDEFAULT, CW_USEDEFAULT, adjSize.width, adjSize.height,
                                     nullptr, nullptr, ::GetModuleHandleW(nullptr), nullptr);
        if (!m_hwnd)
            throw WindowError("Failed to create window!");
        PTK_INFO("Created WinWindow: {}x{}", wSize.width, wSize.height);

        m_context = createWin32Context(backend, m_hwnd, wSize);
        m_data = std::make_unique<WinBackendData>(WinBackendData{window, scale, wSize, {}, style, false, 0, false});
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)m_data.get());

        ::ShowWindow(m_hwnd, SW_SHOW);
        ::UpdateWindow(m_hwnd);
    }

    bool MainWindow_win::setPosHint(const Point& pos)
    {
        if (m_data->pos != pos)
        {
            RECT rc{0};
            if (::GetWindowRect(m_hwnd, &rc))
            {
                if (::MoveWindow(m_hwnd, pos.x, pos.y, rc.right - rc.left, rc.bottom - rc.top, FALSE))
                {
                    m_data->pos = pos;
                    return true;
                }
            }

        }
        return false;
    }

    void MainWindow_win::beginPaint()
    {
        m_ps = PAINTSTRUCT();
        m_hdc = BeginPaint(m_hwnd, &m_ps);
    }

    void MainWindow_win::endPaint()
    {
        ::EndPaint(m_hwnd, &m_ps);
    }

    bool MainWindow_win::setTitle(const std::string& name)
    {
        return ::SetWindowTextW(m_hwnd, Application_win::stringToUTF16(name).c_str());
    }

    bool MainWindow_win::setIcon(int32 width, int32 height, byte* pixels)
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
        if (!color)
        {
            PTK_ASSERT(false, "Failed to create HBITMAP");
            return false;
        }

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
        if (!mask)
        {
            PTK_ASSERT(false, "Failed to create HBITMAP");
            return false;
        }

        // Finally, create the icon.
        ICONINFO iconInfo{true, 0, 0, mask, color};
        HICON hIcon{::CreateIconIndirect(&iconInfo)};
        if (!hIcon)
        {
            PTK_ASSERT(false, "Failed to create the icon");
            return false;
        }

        ::DeleteObject(color);
        ::DeleteObject(mask);

        // Apply icon.
        SendMessage(m_hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(m_hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));

        // Ensure that the application icon is changed.
        SendMessage(::GetWindow(m_hwnd, GW_OWNER), WM_SETICON, ICON_SMALL,
                    reinterpret_cast<LPARAM>(hIcon));
        SendMessage(::GetWindow(m_hwnd, GW_OWNER), WM_SETICON, ICON_BIG,
                    reinterpret_cast<LPARAM>(hIcon));

        return true;
    }

    void MainWindow_win::notifyEvent()
    {
        // Signal the window to exit the event wait.
        PostMessage(m_hwnd, WM_NULL, 0, 0);
    }

    DWORD MainWindow_win::getWindowStyle() const
    {
        return m_data->style;
    }

    void MainWindow_win::swapBuffers()
    {
        m_context->swapBuffers();
    }

    bool MainWindow_win::resize(const Size& size)
    {
        // Apply the DPI scaling.
        const Size scaledSize{scaleSize(size, m_data->scale)};

        // Apply the new size to the context and window.
        if (scaledSize != m_context->getSize())
            m_context->resize(scaledSize);

        if (!m_data->ignoreSize)
        {
            const Size adjSize{calcAdjustedWindowSize(scaledSize, m_data->style, m_data->scale.x * 96.0f)};
            ::SetWindowPos(m_hwnd, 0, 0, 0, adjSize.width, adjSize.height, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
        }

        m_data->ignoreSize = false;
        return true;
    }

    bool MainWindow_win::close()
    {
        return ::DestroyWindow(m_hwnd);
    }

    bool MainWindow_win::show()
    {
        if (!::ShowWindow(m_hwnd, SW_SHOW))
        {
            parent()->forceDrawAll();
            return true;
        }

        return false;
    }

    bool MainWindow_win::hide()
    {
        return ::ShowWindow(m_hwnd, SW_HIDE);
    }

    bool MainWindow_win::isHidden() const
    {
        return !static_cast<bool>(::IsWindowVisible(m_hwnd));
    }

    ContextBase* MainWindow_win::getContext() const
    {
        return m_context.get();
    }

    Vec2f MainWindow_win::getDPIScale() const
    {
        return m_data->scale;
    }

    Point MainWindow_win::getWinPos() const
    {
        return m_data->pos;
    }

    Size MainWindow_win::getWinSize() const
    {
        RECT rect{};
        ::GetWindowRect(m_hwnd, &rect);
        return {rect.right - rect.left, rect.bottom - rect.top};
    }

    bool MainWindow_win::setLimits(const Size&, const Size&)
    {
        RECT rect{};
        ::GetWindowRect(m_hwnd, &rect);
        ::MoveWindow(m_hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
                     TRUE);
        return true;
    }

    bool MainWindow_win::minimize()
    {
        ::ShowWindow(m_hwnd, SW_MINIMIZE);
        return true;
    }

    bool MainWindow_win::isMinimized() const
    {
        return static_cast<bool>(IsMinimized(m_hwnd));
    }

    bool MainWindow_win::restore()
    {
        ::ShowWindow(m_hwnd, SW_RESTORE);
        return true;
    }

    bool MainWindow_win::isFocused() const
    {
        return (m_hwnd == ::GetFocus());
    }

    bool MainWindow_win::setScaleHint(const Vec2f& scale)
    {
        if (m_data->scale != scale)
        {
            m_data->scale = scale;
            resize(parent()->getSize());
            return true;
        }
        return false;
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

    static void handleDPIChange(WinBackendData* data, WPARAM wParam, LPARAM lParam)
    {
        // Change Window position.
        RECT* rect = reinterpret_cast<RECT*>(lParam);
        MoveEvent mEvt{{static_cast<Point::value_type>(rect->left),
                           static_cast<Point::value_type>(rect->top)}};
        data->window->sendEvent(&mEvt);

        // Scale Window.
        const uint32 dpiX{static_cast<uint32>(GET_X_LPARAM(wParam))};
        const uint32 dpiY{static_cast<uint32>(GET_Y_LPARAM(wParam))};
        const Vec2f scale{dpiX / 96.0f, dpiY / 96.0f};
        PTK_INFO("DPI CHANGED {0}x{1} SCALING {2:0.2f}x{3:0.2f}", dpiX, dpiY, scale.x, scale.y);
        ScaleEvent sEvt{scale};
        data->window->sendEvent(&sEvt);
    }

    static void handleWindowLimits(WinBackendData* data, LPARAM lParam)
    {
        Window *window{data->window};
        LPMINMAXINFO lpMMI{reinterpret_cast<LPMINMAXINFO>(lParam)};
        const Size minSize{window->getMinSize()};
        MainWindow_win* backend{static_cast<MainWindow_win*>(window->getBackend())};
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

    static void handleWindowMinimize(WinBackendData* data, bool minimize)
    {
        data->minimized = minimize;
        Event::Type type{(minimize) ? Event::Type::WindowMinimize : Event::Type::WindowRestore};
        Event evt{Event::Category::Window, type};
        data->window->sendEvent(&evt);
    }

    static void handleWindowResize(WinBackendData* data, MainWindowBase *backend, HWND hwnd)
    {
        Window *window{data->window};
        RECT rc{0};
        if (::GetClientRect(hwnd, &rc) && (rc.right != 0 && rc.bottom != 0))
        {
            if (rc.right != data->size.width || rc.bottom != data->size.height)
            {
                if (backend)
                {
                    const Size size{static_cast<Size::value_type>(rc.right),
                                    static_cast<Size::value_type>(rc.bottom)};
                    ResizeEvent evt{scaleSize(size,
                                              Vec2f{1.0f / data->scale.x,
                                                    1.0f / data->scale.y})};
                    data->ignoreSize = true;
                    window->sendEvent(&evt);
                    window->forceDrawAll();
                }
            }
        }
    }

    LRESULT MainWindow_win::wndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        WinBackendData* data{
            reinterpret_cast<WinBackendData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))};
        Window* window{(data) ? data->window : nullptr};
        switch (msg)
        {
            case WM_CREATE:
                break;
            case WM_CLOSE:
                ::DestroyWindow(hwnd);
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
                ResizeEvent evt{scaleSize(calcAdjustedReverseWindowSize(size, data->style, data->scale.x * 96.0f),
                                          Vec2f{1.0f / data->scale.x, 1.0f / data->scale.y})};
                if (evt.size != data->size)
                {
                    data->ignoreSize = true;
                    window->sendEvent(&evt);
                    window->forceDrawAll();
                }
                break;
            }
            case WM_GETMINMAXINFO:
            {
                if (window)
                    handleWindowLimits(data, lParam);
                break;
            }
            case WM_DPICHANGED:
            {
                handleDPIChange(data, wParam, lParam);
                break;
            }
            case WM_WINDOWPOSCHANGED:
            {
                if (window)
                {
                    WINDOWPOS* winData{reinterpret_cast<WINDOWPOS*>(lParam)};
                    MainWindowBase* backend{window->getBackend()};

                    // Window was moved.
                    if (!(winData->flags & SWP_NOMOVE))
                    {
                        if (winData->x != data->pos.x || winData->y != data->pos.y)
                        {
                            MoveEvent evt{{winData->x, winData->y}};
                            window->sendEvent(&evt);
                        }
                    }

                    // Window was resized.
                    if (!(winData->flags & SWP_NOSIZE))
                        handleWindowResize(data, backend, hwnd);

                    // Window was shown.
                    if (!(winData->flags & SWP_SHOWWINDOW) && backend)
                        if (!backend->isMinimized() && data->minimized)
                            handleWindowMinimize(data, false);

                    // Window was hidden.
                    if (!(winData->flags & SWP_HIDEWINDOW) && backend)
                        if (backend->isMinimized() && !data->minimized)
                            handleWindowMinimize(data, true);
                }
                break;
            }
            case WM_ENTERSIZEMOVE:
            {
                SetTimer(hwnd, 1, data->wait, nullptr);
                break;
            }
            case WM_EXITSIZEMOVE:
            {
                KillTimer(hwnd, 1);
                break;
            }
            case WM_TIMER:
            {
                if (wParam == 1)
                {
                    window->handleEvents();
                }
                break;
            }
            default:
                return ::DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        return 0;
    }
} // namespace pTK
