//
//  platform/win/WindowHandle_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "WindowHandle_win.hpp"
#include "ApplicationHandle_win.hpp"
#include "RasterPolicy_win.hpp"

// Include OpenGL backend if HW Acceleration is enabled.
#ifdef PTK_OPENGL
    #include "GLContext_win.hpp"
#endif // PTK_OPENGL

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/events/KeyMap.hpp"
#include "ptk/menu/NamedMenuItem.hpp"
#include "ptk/platform/common/RasterContext.hpp"

// Windows Headers
#include <Dwmapi.h>
#include <windowsx.h>

// C++ Headers
#include <cmath>
#include <memory>
#include <mutex>
#include <tuple>

namespace pTK::Platform
{
    // Since the iHandleEvent function is protected in WindowHandle, this is a friend function
    // to get around that issue. Maybe another way is better in the future, but this works
    // for now. This must be done since the win32 WndPro is a static function and only a
    // pointer to window can be used (in this implementation anyway).
    template <typename Event>
    void EventSendHelper(WindowHandle_win* window, const Event& evt)
    {
        window->HandlePlatformEvent<Event>(evt);
    }

    Limits GetWindowLimits(WindowHandle_win* window)
    {
        return window->winBase()->getLimitsWithSizePolicy();
    }

    ///////////////////////////////////////////////////////////////////////////////

    static Size CalcAdjustedReverseWindowSize(const Size& from, DWORD style, bool menu, float dpi)
    {
        RECT adjustedSize{};
        ::SetRectEmpty(&adjustedSize);
        ::AdjustWindowRectExForDpi(&adjustedSize, style, menu, 0, static_cast<UINT>(dpi));

        return {from.width - (adjustedSize.right - adjustedSize.left),
                from.height - (adjustedSize.bottom - adjustedSize.top)};
    }

    static Size CalcAdjustedWindowSize(const Size& from, DWORD style, bool menu, float dpi)
    {
        RECT adjustedSize{};
        adjustedSize.top = 0;
        adjustedSize.bottom = static_cast<long>(from.height);
        adjustedSize.left = 0;
        adjustedSize.right = static_cast<long>(from.width);
        ::AdjustWindowRectExForDpi(&adjustedSize, style, menu, 0, static_cast<UINT>(dpi));
        return {static_cast<Size::value_type>(adjustedSize.right - adjustedSize.left),
                static_cast<Size::value_type>(adjustedSize.bottom - adjustedSize.top)};
    }

    static Size ScaleSize(const Size& size, const Vec2f& scale) noexcept
    {
        const float width{std::ceil(static_cast<float>(size.width) * scale.x)};
        const float height{std::ceil(static_cast<float>(size.height) * scale.y)};
        return {static_cast<Size::value_type>(width), static_cast<Size::value_type>(height)};
    }

    ///////////////////////////////////////////////////////////////////////////////

    WindowHandle_win::WindowHandle_win(WindowBase* base, const std::string& name, const Size& size,
                                       const WindowInfo& flags)
        : WindowHandle(base)
    {
        m_data.window = this;

        // Menubar.
        m_data.hasMenu = (static_cast<bool>(flags.menus) && !flags.menus->empty());

        // High DPI.
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
        PTK_INFO("System DPI {}x{}", dpiX, dpiY);

        m_scale = {dpiX / 96.0f, dpiY / 96.0f};
        Size scaledSize = ScaleSize(size, m_scale);
        const Size adjSize{CalcAdjustedWindowSize(scaledSize, m_data.style, m_data.hasMenu, dpiX)};
        // Old position was set to CW_USEDEFAULT (replaced with flags.position).
        const int width{static_cast<int>(adjSize.width)};
        const int height{static_cast<int>(adjSize.height)};
        m_hwnd = ::CreateWindowExW(0, L"PTK", ApplicationHandle_win::stringToUTF16(name).c_str(), m_data.style,
                                   flags.position.x, flags.position.y, width, height, nullptr, nullptr,
                                   ::GetModuleHandleW(nullptr), nullptr);
        if (!m_hwnd)
            throw WindowError("Failed to create window!");

        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)&m_data);

        if (m_data.hasMenu)
        {
            HMENU menuBar = ::CreateMenu();
            uint32_t menuBarId = MenuBarUtil_win::InsertMenuItemToMap(m_data.menuItems, nullptr, 1, true, menuBar);
            std::vector<ACCEL> accelShortcuts{};

            for (auto menuIt{flags.menus->cbegin()}; menuIt != flags.menus->cend(); ++menuIt)
                MenuBarUtil_win::CreateMenuStructure(menuBar, m_data.menuItems, (*menuIt), menuBarId, accelShortcuts);

            ::SetMenu(m_hwnd, menuBar);

            m_accelTable = ::CreateAcceleratorTableW(accelShortcuts.data(), static_cast<int>(accelShortcuts.size()));
        }

        PTK_INFO("Initialized WindowHandle_win {}x{} at {}x{}", size.width, size.height, flags.position.x,
                 flags.position.y);
    }

    WindowHandle_win::~WindowHandle_win()
    {
        destroyWindow();
        PTK_INFO("Destroyed WindowHandle_win");
    }

    bool WindowHandle_win::destroyWindow()
    {
        bool ret = false;

        if (m_accelTable != nullptr)
        {
            for (auto it = m_data.menuItems.rbegin(); it != m_data.menuItems.rend(); ++it)
            {
                bool isMenu = std::get<2>(it->second);
                if (isMenu)
                {
                    HMENU hmenu = std::get<3>(it->second);
                    ::DestroyMenu(hmenu);
                }
            }

            ::DestroyAcceleratorTable(m_accelTable);
            m_accelTable = nullptr;
        }

        if (m_hwnd)
        {
            ret = ::DestroyWindow(m_hwnd);
            m_hwnd = nullptr;
        }

        return ret;
    }

    bool WindowHandle_win::setPosHint(const Point& pos)
    {
        RECT rc{};
        if (::GetWindowRect(m_hwnd, &rc))
            return ::MoveWindow(m_hwnd, pos.x, pos.y, rc.right - rc.left, rc.bottom - rc.top, FALSE);

        return false;
    }

    void WindowHandle_win::beginPaint()
    {
        m_ps = PAINTSTRUCT();
        m_hdc = BeginPaint(m_hwnd, &m_ps);
    }

    void WindowHandle_win::endPaint()
    {
        ::EndPaint(m_hwnd, &m_ps);
    }

    bool WindowHandle_win::setTitle(const std::string& name)
    {
        return ::SetWindowTextW(m_hwnd, ApplicationHandle_win::stringToUTF16(name).c_str());
    }

    bool WindowHandle_win::setIcon(int32_t width, int32_t height, byte* pixels)
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
        SendMessage(::GetWindow(m_hwnd, GW_OWNER), WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIcon));
        SendMessage(::GetWindow(m_hwnd, GW_OWNER), WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIcon));

        return true;
    }

    void WindowHandle_win::notifyEvent()
    {
        // Signal the window to exit the event wait.
        PostMessage(m_hwnd, WM_NULL, 0, 0);
    }

    DWORD WindowHandle_win::getWindowStyle() const
    {
        return m_data.style;
    }

    bool WindowHandle_win::resize(const Size& size)
    {
        // Apply the DPI scaling.
        const Vec2f scale{getDPIScale()};
        const Size scaledSize{ScaleSize(size, getDPIScale())};
        const Size adjSize{CalcAdjustedWindowSize(scaledSize, getWindowStyle(), m_data.hasMenu, scale.x * 96.0f)};

        // Apply the new size to the context and window.
        if (scaledSize != getSize())
        {
            if (!m_data.ignoreSize)
            {
                const int width{static_cast<int>(adjSize.width)};
                const int height{static_cast<int>(adjSize.height)};
                ::SetWindowPos(m_hwnd, nullptr, 0, 0, width, height,
                               SWP_NOSENDCHANGING | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);
            }
        }

        m_data.ignoreSize = false;
        return true;
    }

    bool WindowHandle_win::close()
    {
        return ::CloseWindow(m_hwnd);
    }

    void WindowHandle_win::show()
    {
        ::ShowWindow(m_hwnd, SW_SHOW);
        EventSendHelper<PaintEvent>(this, {{0, 0}, getSize()});
    }

    void WindowHandle_win::hide()
    {
        ::ShowWindow(m_hwnd, SW_HIDE);
    }

    bool WindowHandle_win::isHidden() const
    {
        return !static_cast<bool>(::IsWindowVisible(m_hwnd));
    }

    Vec2f WindowHandle_win::getDPIScale() const
    {
        return m_scale;
    }

    Point WindowHandle_win::getPosition() const
    {
        RECT rect{};
        ::GetWindowRect(m_hwnd, &rect);
        return {static_cast<Point::value_type>(rect.left), static_cast<Point::value_type>(rect.top)};
    }

    Size WindowHandle_win::getSize() const
    {
        RECT rect{};
        ::GetWindowRect(m_hwnd, &rect);
        return {static_cast<Size::value_type>(rect.right - rect.left),
                static_cast<Size::value_type>(rect.bottom - rect.top)};
    }

    void WindowHandle_win::setLimits([[maybe_unused]] const Size& min, [[maybe_unused]] const Size& max)
    {
        PTK_INFO("Updating Window limits to: min: {}x{} max: {}x{}", min.width, min.height, max.width, max.height);
        RECT rect{};
        ::GetWindowRect(m_hwnd, &rect);
        ::MoveWindow(m_hwnd, rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top, FALSE);
    }

    bool WindowHandle_win::minimize()
    {
        ::ShowWindow(m_hwnd, SW_MINIMIZE);
        return true;
    }

    bool WindowHandle_win::isMinimized() const
    {
        return static_cast<bool>(IsMinimized(m_hwnd));
    }

    bool WindowHandle_win::restore()
    {
        ::ShowWindow(m_hwnd, SW_RESTORE);
        return true;
    }

    bool WindowHandle_win::isFocused() const
    {
        return (m_hwnd == ::GetFocus());
    }

    bool WindowHandle_win::setScaleHint(const Vec2f& scale)
    {
        if (m_scale != scale)
        {
            m_scale = scale;
            resize(getSize());
            return true;
        }
        return false;
    }

    void WindowHandle_win::inval()
    {
        if (!m_data.invalidated)
            InvalidateRect(m_hwnd, nullptr, false);
        m_data.invalidated = false;
    }

    HWND WindowHandle_win::handle() const
    {
        return m_hwnd;
    }

    HACCEL WindowHandle_win::accelTable() const
    {
        return m_accelTable;
    }

    static void HandleMouseClick(WindowHandle_win* window, const Vec2f& scale, Event::Type type, Mouse::Button btn,
                                 int32_t value, LPARAM lParam)
    {
        PTK_ASSERT(window, "WindowHandle_win pointer is undefined");

        const Point pos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
        const auto fX = static_cast<float>(pos.x);
        const auto fY = static_cast<float>(pos.y);
        const Point scaledPos{static_cast<Point::value_type>(fX * (1 / scale.x)),
                              static_cast<Point::value_type>(fY * (1 / scale.y))};

        if (type == Event::Type::MouseButtonPressed)
        {
            ClickEvent evt{btn, value, scaledPos};
            EventSendHelper<ClickEvent>(window, evt);
        }
        else
        {
            ReleaseEvent evt{btn, value, scaledPos};
            EventSendHelper<ReleaseEvent>(window, evt);
        }
    }

    static void HandleDPIChange(WindowHandle_win* window, WPARAM wParam, LPARAM lParam)
    {
        PTK_ASSERT(window, "WindowHandle_win pointer is undefined");

        // Change Window position.
        RECT* rect = reinterpret_cast<RECT*>(lParam);
        MoveEvent mEvt{{static_cast<Point::value_type>(rect->left), static_cast<Point::value_type>(rect->top)}};
        EventSendHelper<MoveEvent>(window, mEvt);

        // Scale Window.
        const auto dpiX{static_cast<uint32_t>(GET_X_LPARAM(wParam))};
        const auto dpiY{static_cast<uint32_t>(GET_Y_LPARAM(wParam))};
        const Vec2f scale{static_cast<float>(dpiX) / 96.0f, static_cast<float>(dpiY) / 96.0f};
        PTK_INFO("DPI CHANGED {0}x{1} SCALING {2:0.2f}x{3:0.2f}", dpiX, dpiY, scale.x, scale.y);
        ScaleEvent sEvt{scale};
        EventSendHelper<ScaleEvent>(window, sEvt);
    }

    static void HandleWindowLimits(WindowHandle_win* window, LPARAM lParam, const Vec2f& scale, bool hasMenu,
                                   DWORD style)
    {
        PTK_ASSERT(window, "WindowHandle_win pointer is undefined");

        LPMINMAXINFO lpMMI{reinterpret_cast<LPMINMAXINFO>(lParam)};
        const Limits limits{GetWindowLimits(window)};

        const Size adjMinSize{
            CalcAdjustedWindowSize(ScaleSize(limits.min, scale), window->getWindowStyle(), hasMenu, scale.x * 96.0f)};
        lpMMI->ptMinTrackSize.x = static_cast<int>(adjMinSize.width);
        lpMMI->ptMinTrackSize.y = static_cast<int>(adjMinSize.height);

        if (limits.max != Size::Max)
        {
            const Size adjMaxSize{
                CalcAdjustedWindowSize(ScaleSize(limits.max, scale), style, hasMenu, scale.x * 96.0f)};
            lpMMI->ptMaxTrackSize.x = static_cast<int>(adjMaxSize.width);
            lpMMI->ptMaxTrackSize.y = static_cast<int>(adjMaxSize.height);
        }
        else
        {
            lpMMI->ptMaxTrackSize.x = ::GetSystemMetrics(SM_CXMAXTRACK);
            lpMMI->ptMaxTrackSize.y = ::GetSystemMetrics(SM_CYMAXTRACK);
        }

        if (limits.min == limits.max)
            ::SetWindowLong(window->handle(), GWL_STYLE, GetWindowLong(window->handle(), GWL_STYLE) & ~WS_SIZEBOX);
        else
            ::SetWindowLong(window->handle(), GWL_STYLE, GetWindowLong(window->handle(), GWL_STYLE) | WS_SIZEBOX);
    }

    static void HandleWindowMinimize(WindowHandle_win* window, WindowHandle_win::Data* data, bool minimize)
    {
        PTK_ASSERT(window, "WindowHandle_win pointer is undefined");
        PTK_ASSERT(data, "WindowHandle_win data pointer is undefined");

        data->minimized = minimize;

        if (minimize)
            EventSendHelper<MinimizeEvent>(window, {});
        else
            EventSendHelper<RestoreEvent>(window, {});
    }

    static void HandleWindowResize(WindowHandle_win* window, WindowHandle_win::Data* data, HWND hwnd)
    {
        PTK_ASSERT(window, "WindowHandle_win pointer is undefined");
        PTK_ASSERT(data, "WindowHandle_win data pointer is undefined");

        RECT rc{};
        if (::GetClientRect(hwnd, &rc) && (rc.right != 0 && rc.bottom != 0))
        {
            const Vec2f scale = window->getDPIScale();
            const Size size = window->getSize();
            if (static_cast<Size::value_type>(rc.right) != size.width ||
                static_cast<Size::value_type>(rc.bottom) != size.height)
            {
                const Size rSize{static_cast<Size::value_type>(rc.right), static_cast<Size::value_type>(rc.bottom)};
                ResizeEvent evt{ScaleSize(rSize, Vec2f{1.0f / scale.x, 1.0f / scale.y})};
                data->ignoreSize = true;
                EventSendHelper<ResizeEvent>(window, evt);
            }
        }
    }

    static constexpr WPARAM MapLeftRightKeys(WPARAM wParam, LPARAM lParam) noexcept
    {
        WPARAM key = wParam;
        uint32_t scancode = (lParam & 0x00ff0000) >> 16;
        int extended = (lParam & 0x01000000) != 0;

        switch (wParam)
        {
            case VK_SHIFT:
                key = MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX);
                break;
            case VK_CONTROL:
                key = extended ? VK_RCONTROL : VK_LCONTROL;
                break;
            case VK_MENU:
                key = extended ? VK_RMENU : VK_LMENU;
                break;
            default:
                break;
        }

        return key;
    }

    static std::underlying_type<KeyEvent::Modifier>::type GetKeyModifiers()
    {
        using utype = std::underlying_type<KeyEvent::Modifier>::type;
        auto mods = static_cast<utype>(KeyEvent::Modifier::NONE);

        if (GetKeyState(VK_SHIFT) & 0x800)
            mods |= static_cast<utype>(KeyEvent::Modifier::Shift);

        if (GetKeyState(VK_CONTROL) & 0x800)
            mods |= static_cast<utype>(KeyEvent::Modifier::Control);

        if (GetKeyState(VK_MENU) & 0x800)
            mods |= static_cast<utype>(KeyEvent::Modifier::Alt);

        if ((GetKeyState(VK_LWIN) | GetKeyState(VK_RWIN)) & 0x8000)
            mods |= static_cast<utype>(KeyEvent::Modifier::Super);

        if (GetKeyState(VK_CAPITAL) & 1)
            mods |= static_cast<utype>(KeyEvent::Modifier::CapsLock);

        if (GetKeyState(VK_NUMLOCK) & 1)
            mods |= static_cast<utype>(KeyEvent::Modifier::NumLock);

        return mods;
    }

    static void HandleKeyEvent(WindowHandle_win* window, const Event::Type& type, WPARAM wParam, LPARAM lParam)
    {
        PTK_ASSERT(window, "WindowHandle_win pointer is undefined");

        const auto data{static_cast<int32_t>(wParam)};
        KeyCode key{KeyMap::KeyCodeToKey(data)};

        if (key == Key::Unknown)
        {
            WPARAM lrKey{MapLeftRightKeys(wParam, lParam)};
            key = KeyMap::KeyCodeToKey(static_cast<int32_t>(lrKey));
        }

        KeyEvent evt{type, key, static_cast<uint32_t>(data), GetKeyModifiers()};
        EventSendHelper<KeyEvent>(window, evt);
    }

    static void HandleCharInput(WindowHandle_win* window, WPARAM wParam, LPARAM UNUSED(lParam))
    {
        PTK_ASSERT(window, "WindowHandle_win pointer is undefined");

        uint32_t data{0};

        switch (wParam)
        {
            case 0x08: // Backspace
            case 0x0A: // Linefeed
            case 0x1B: // Escape
            case 0x09: // Tab
            case 0x0D: // Carriage return
            {
                // Ignore these for now.
                // TODO: Decide on how to fix this (includes these or not?).
                break;
            }
            default:
            {
                // Displayable character.
                data = static_cast<uint32_t>(wParam);
                break;
            }
        }

        if (data > 0)
        {
            pTK::InputEvent::data_cont arr(new pTK::InputEvent::data_type[1]);
            arr[0] = data;

            InputEvent evt{arr, 1, Text::Encoding::UTF16};
            EventSendHelper<InputEvent>(window, evt);
        }
    }

    LRESULT WindowHandle_win::WndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        Data* data{reinterpret_cast<Data*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))};
        if (!data)
            return ::DefWindowProcW(hwnd, msg, wParam, lParam);

        WindowHandle_win* window = data->window;
        PTK_ASSERT(window, "WindowHandle_win pointer is undefined");

        switch (msg)
        {
            case WM_CREATE:
                break;
            case WM_CLOSE:
            {
                // Window close request should be sent here.
                // To enable the window to halt the closing request.

                // window->handleEvents(); // Handle all events before sending close event.
                EventSendHelper<CloseEvent>(window, {});
                if (auto win = dynamic_cast<Window*>(window->winBase()))
                    Application::Get()->removeWindow(win);
                DestroyWindow(hwnd);
                break;
            }
            case WM_DESTROY:
            {
                window->destroyWindow();
                break;
            }
            case WM_SETFOCUS:
            {
                EventSendHelper<FocusEvent>(window, {});
                break;
            }
            case WM_KILLFOCUS:
            {
                EventSendHelper<LostFocusEvent>(window, {});
                break;
            }
            case WM_PAINT:
            {
                data->invalidated = true;
                EventSendHelper<PaintEvent>(window, {{0, 0}, window->getSize()});
                break;
            }
            case WM_CHAR:
            {
                HandleCharInput(window, wParam, lParam);
                break;
            }
            case WM_SYSKEYDOWN:
            case WM_KEYDOWN:
            {
                HandleKeyEvent(window, KeyEvent::Pressed, wParam, lParam);
                break;
            }
            case WM_SYSKEYUP:
            case WM_KEYUP:
            {
                HandleKeyEvent(window, KeyEvent::Released, wParam, lParam);
                break;
            }
            case WM_MOUSEMOVE:
            {
                const auto fX = static_cast<float>(GET_X_LPARAM(lParam));
                const auto fY = static_cast<float>(GET_Y_LPARAM(lParam));
                const Vec2f scale = window->getDPIScale();
                MotionEvent evt{{static_cast<Point::value_type>(fX * (1 / scale.x)),
                                 static_cast<Point::value_type>(fY * (1 / scale.y))}};
                EventSendHelper<MotionEvent>(window, evt);
                break;
            }
            case WM_LBUTTONDOWN:
            case WM_RBUTTONDOWN:
            {
                const Vec2f scale = window->getDPIScale();
                const Mouse::Button btn = (msg == WM_LBUTTONDOWN) ? Mouse::Button::Left : Mouse::Button::Right;
                const int32_t value = (msg == WM_LBUTTONDOWN) ? VK_LBUTTON : VK_RBUTTON;
                HandleMouseClick(window, scale, Event::Type::MouseButtonPressed, btn, value, lParam);
                break;
            }
            case WM_LBUTTONUP:
            case WM_RBUTTONUP:
            {
                const Vec2f scale = window->getDPIScale();
                const Mouse::Button btn = (msg == WM_LBUTTONUP) ? Mouse::Button::Left : Mouse::Button::Right;
                const int32_t value = (msg == WM_LBUTTONUP) ? VK_LBUTTON : VK_RBUTTON;
                HandleMouseClick(window, scale, Event::Type::MouseButtonReleased, btn, value, lParam);
                break;
            }
            case WM_MOUSEWHEEL:
            {
                const float y_offset =
                    static_cast<float>(static_cast<SHORT>(HIWORD(wParam))) / static_cast<float>(WHEEL_DELTA);
                EventSendHelper<ScrollEvent>(window, ScrollEvent{{0.0f, y_offset}});
                break;
            }
            case WM_MOUSEHWHEEL:
            {
                const float x_offset =
                    -static_cast<float>(static_cast<SHORT>(HIWORD(wParam))) / static_cast<float>(WHEEL_DELTA);
                EventSendHelper<ScrollEvent>(window, ScrollEvent{{x_offset, 0.0f}});
                break;
            }
            case WM_XBUTTONUP:
            case WM_XBUTTONDOWN:
            {
                const Vec2f scale = window->getDPIScale();
                const Event::Type type =
                    (msg == WM_XBUTTONUP) ? Event::Type::MouseButtonReleased : Event::Type::MouseButtonPressed;
                const Mouse::Button btn =
                    (GET_XBUTTON_WPARAM(wParam) == XBUTTON1) ? Mouse::Button::Back : Mouse::Button::Forward;
                const int32_t value = (btn == Mouse::Button::Back) ? VK_XBUTTON1 : VK_XBUTTON2;
                HandleMouseClick(window, scale, type, btn, value, lParam);
                break;
            }
            case WM_SIZING:
            {
                RECT* rect = reinterpret_cast<RECT*>(lParam);
                const Size size = {static_cast<Size::value_type>(rect->right - rect->left),
                                   static_cast<Size::value_type>(rect->bottom - rect->top)};
                const Vec2f scale = window->getDPIScale();
                DWORD style = window->getWindowStyle();
                ResizeEvent evt{ScaleSize(CalcAdjustedReverseWindowSize(size, style, data->hasMenu, scale.x * 96.0f),
                                          Vec2f{1.0f / scale.x, 1.0f / scale.y})};

                data->ignoreSize = true;
                // EventSendHelper<ResizeEvent>(window, evt);
                break;
            }
            case WM_GETMINMAXINFO:
            {
                const Vec2f scale = window->getDPIScale();
                DWORD style = window->getWindowStyle();
                HandleWindowLimits(window, lParam, scale, data->hasMenu, style);
                break;
            }
            case WM_DPICHANGED:
            {
                HandleDPIChange(window, wParam, lParam);
                break;
            }
            case WM_WINDOWPOSCHANGED:
            {
                WINDOWPOS* winData{reinterpret_cast<WINDOWPOS*>(lParam)};

                // Window was moved.
                if (!(winData->flags & SWP_NOMOVE))
                {
                    MoveEvent evt{{winData->x, winData->y}};
                    EventSendHelper<MoveEvent>(window, evt);
                }

                // Window was resized.
                if (!(winData->flags & SWP_NOSIZE))
                    HandleWindowResize(window, data, hwnd);

                // Window was shown.
                if (!(winData->flags & SWP_SHOWWINDOW))
                    if (!window->isMinimized() && data->minimized)
                        HandleWindowMinimize(window, data, false);

                // Window was hidden.
                if (!(winData->flags & SWP_HIDEWINDOW))
                    if (window->isMinimized() && !data->minimized)
                        HandleWindowMinimize(window, data, true);

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
                    if (auto win = reinterpret_cast<Window*>(window->winBase()))
                        win->handleEvents();
                break;
            }
            case WM_COMMAND:
            {
                uint32_t wmId{LOWORD(wParam)};
                Ref<MenuItem> item{MenuBarUtil_win::FindMenuItemById(data->menuItems, wmId)};

                if (item)
                {
                    auto* nItem = dynamic_cast<NamedMenuItem*>(item.get());
                    if (nItem)
                        nItem->notifyClick();
                }
                else
                {
                    return ::DefWindowProcW(hwnd, msg, wParam, lParam);
                }
                break;
            }
            default:
                return ::DefWindowProcW(hwnd, msg, wParam, lParam);
        }

        return 0;
    }
} // namespace pTK
