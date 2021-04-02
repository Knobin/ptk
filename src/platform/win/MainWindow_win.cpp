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
#include "MenuBarUtil_win.hpp"

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/events/KeyMap.hpp"
#include "ptk/menu/NamedMenuItem.hpp"

// Include OpenGL backend if HW Acceleration is enabled.
#ifdef PTK_OPENGL
#include "GLContext_win.hpp"
#endif // PTK_OPENGL

// Windows Headers
#include <windowsx.h>
#include <Dwmapi.h>

// C++ Headers
#include <cmath>
#include <memory>
#include <tuple>
#include <mutex>

namespace pTK
{
    static std::unique_ptr<ContextBase> CreateContextForWin32([[maybe_unused]] WindowInfo::Backend type, HWND hwnd,
                                                           const Size& size)
    {
#ifdef PTK_OPENGL
        if (type == WindowInfo::Backend::Hardware)
            return std::make_unique<GLContext_win>(hwnd, size);
#endif // PTK_OPENGL

        // Software backend is always available.
        return std::make_unique<RasterContext<RasterPolicy_win>>(size, RasterPolicy_win{hwnd});
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
        adjustedSize.bottom = from.height;
        adjustedSize.left = 0;
        adjustedSize.right = from.width;
        ::AdjustWindowRectExForDpi (&adjustedSize, style, menu, 0, static_cast<UINT>(dpi));
        return {adjustedSize.right - adjustedSize.left, adjustedSize.bottom - adjustedSize.top};
    }

    static Size ScaleSize(const Size& size, const Vec2f& scale) noexcept
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
        bool hasMenu;
        MenuBarUtil_win::MenuMap menuItems;
        HACCEL accelTable{nullptr};
    };

    ///////////////////////////////////////////////////////////////////////////////
    
    MainWindow_win::MainWindow_win(Window *window, const std::string& name, const Size& size, const WindowInfo& flags)
        : MainWindowBase(window)
    {
        // Default window data.
        m_data = std::make_unique<WinBackendData>(WinBackendData{window});
        m_data->minimized = false;
        m_data->wait = 0;
        m_data->ignoreSize = false;
        m_data->pos = flags.position;

        // Menubar.
        m_data->hasMenu = (static_cast<bool>(flags.menus) && !flags.menus->empty());

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

        m_data->scale = {dpiX / 96.0f, dpiY / 96.0f};
        m_data->size = ScaleSize(size, m_data->scale);
        m_data->style = WS_OVERLAPPEDWINDOW | WS_THICKFRAME | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX;
        const Size adjSize{CalcAdjustedWindowSize(m_data->size, m_data->style, m_data->hasMenu, dpiX)};
        // Old position was set to CW_USEDEFAULT (replaced with flags.position).
        m_hwnd = ::CreateWindowExW(0, L"PTK", Application_win::stringToUTF16(name).c_str(), m_data->style,
                                    flags.position.x, flags.position.y, adjSize.width, adjSize.height,
                                     nullptr, nullptr, ::GetModuleHandleW(nullptr), nullptr);
        if (!m_hwnd)
            throw WindowError("Failed to create window!");

        m_context = CreateContextForWin32(flags.backend, m_hwnd, m_data->size);
        SetWindowLongPtr(m_hwnd, GWLP_USERDATA, (LONG_PTR)m_data.get());

        switch (flags.visibility)
        {
            case WindowInfo::Visibility::Windowed:
            {
                ::ShowWindow(m_hwnd, SW_SHOW);
                ::UpdateWindow(m_hwnd);
                break;
            }
            case WindowInfo::Visibility::Hidden:
            {
                ::ShowWindow(m_hwnd, SW_HIDE);
                break;
            }
            default:
                break;
        }

        if (m_data->hasMenu)
        {
            HMENU menuBar = ::CreateMenu();
            uint menuBarId = MenuBarUtil_win::InsertMenuItemToMap(m_data->menuItems, nullptr, 1, true, menuBar);
            std::vector<ACCEL> accelShortcuts{};

            for (auto menuIt{flags.menus->cbegin()}; menuIt != flags.menus->cend(); ++menuIt)
                MenuBarUtil_win::CreateMenuStructure(menuBar, m_data->menuItems, (*menuIt), menuBarId, accelShortcuts);

            ::SetMenu(m_hwnd, menuBar);

            m_data->accelTable = ::CreateAcceleratorTableW(accelShortcuts.data(), static_cast<int>(accelShortcuts.size()));
        }

        PTK_INFO("Initialized MainWindow_win {}x{} at {}x{}", m_data->size.width, m_data->size.height, flags.position.x, flags.position.y);
    }

    MainWindow_win::~MainWindow_win()
    {
        for (auto it = m_data->menuItems.rbegin(); it != m_data->menuItems.rend(); ++it)
        {
            bool isMenu = std::get<2>(it->second);
            if (isMenu)
            {
                HMENU hmenu = std::get<3>(it->second);
                ::DestroyMenu(hmenu);
            }
        }

        ::DestroyAcceleratorTable(m_data->accelTable);
        m_data->accelTable = nullptr;

        PTK_INFO("Destroyed MainWindow_win");
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
        const Size scaledSize{ScaleSize(size, m_data->scale)};

        // Apply the new size to the context and window.
        if (scaledSize != m_context->getSize())
            m_context->resize(scaledSize);

        if (!m_data->ignoreSize)
        {
            const Size adjSize{CalcAdjustedWindowSize(scaledSize, m_data->style, m_data->hasMenu, m_data->scale.x * 96.0f)};
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

    HWND MainWindow_win::handle() const
    {
        return m_hwnd;
    }

    HACCEL MainWindow_win::accelTable() const
    {
        return m_data->accelTable;
    }

    static void HandleMouseClick(WinBackendData* data, Event::Type type, Mouse::Button btn,
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

    static void HandleDPIChange(WinBackendData* data, WPARAM wParam, LPARAM lParam)
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

    static void HandleWindowLimits(WinBackendData* data, LPARAM lParam)
    {
        Window *window{data->window};
        LPMINMAXINFO lpMMI{reinterpret_cast<LPMINMAXINFO>(lParam)};
        const Size minSize{window->getMinSize()};
        MainWindow_win* backend{static_cast<MainWindow_win*>(window->getBackend())};
        const Size adjMinSize{CalcAdjustedWindowSize(ScaleSize(minSize, data->scale),
                                                     backend->getWindowStyle(), data->hasMenu, data->scale.x * 96.0f)};
        lpMMI->ptMinTrackSize.x = adjMinSize.width;
        lpMMI->ptMinTrackSize.y = adjMinSize.height;
        const Size maxSize{window->getMaxSize()};
        if (maxSize != Size::Max)
        {
            const Size adjMaxSize{CalcAdjustedWindowSize(
                ScaleSize(maxSize, data->scale), data->style, data->hasMenu, data->scale.x * 96.0f)};
            lpMMI->ptMaxTrackSize.x = adjMaxSize.width;
            lpMMI->ptMaxTrackSize.y = adjMaxSize.height;
        }
        else
        {
            lpMMI->ptMaxTrackSize.x = ::GetSystemMetrics(SM_CXMAXTRACK);
            lpMMI->ptMaxTrackSize.y = ::GetSystemMetrics(SM_CYMAXTRACK);
        }
    }

    static void HandleWindowMinimize(WinBackendData* data, bool minimize)
    {
        data->minimized = minimize;
        Event::Type type{(minimize) ? Event::Type::WindowMinimize : Event::Type::WindowRestore};
        Event evt{Event::Category::Window, type};
        data->window->sendEvent(&evt);
    }

    static void HandleWindowResize(WinBackendData* data, MainWindowBase *backend, HWND hwnd)
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
                    ResizeEvent evt{ScaleSize(size,
                                              Vec2f{1.0f / data->scale.x,
                                                    1.0f / data->scale.y})};
                    data->ignoreSize = true;
                    window->sendEvent(&evt);
                    window->forceDrawAll();
                }
            }
        }
    }

    static constexpr WPARAM MapLeftRightKeys(WPARAM wParam, LPARAM lParam) noexcept
    {
        WPARAM key = wParam;
        uint scancode = (lParam & 0x00ff0000) >> 16;
        int extended  = (lParam & 0x01000000) != 0;

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

    static void HandleKeyEvent(Window *window, const Event::Type& type, WPARAM wParam, LPARAM lParam)
    {
        KeyCode key{KeyMap::KeyCodeToKey(static_cast<int32>(wParam))};
        if (key == Key::Unknown)
        {
            WPARAM lrKey{MapLeftRightKeys(wParam, lParam)};
            key = KeyMap::KeyCodeToKey(static_cast<int32>(lrKey));
        }

        KeyEvent evt{type, key};
        window->sendEvent(&evt);
    }

    LRESULT MainWindow_win::WndPro(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        WinBackendData* data{
            reinterpret_cast<WinBackendData*>(GetWindowLongPtr(hwnd, GWLP_USERDATA))};
        if (!data)
            return ::DefWindowProcW(hwnd, msg, wParam, lParam);
        Window* window{data->window};

        switch (msg)
        {
            case WM_CREATE:
                break;
            case WM_CLOSE:
            {
                Event evt{Event::Category::Window, Event::Type::WindowClose};
                window->handleEvents(); // Handle all events before sending close event.
                window->sendEvent(&evt); // Calls the ::DestroyWindow function.
                break;
            }
            case WM_DESTROY:
            {
                Application::Get()->removeWindow(window);
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
                const Point pos{GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
                MotionEvent evt{{static_cast<Point::value_type>(pos.x * (1 / data->scale.x)),
                                 static_cast<Point::value_type>(pos.y * (1 / data->scale.y))}};
                window->sendEvent(&evt);
                break;
            }
            case WM_LBUTTONDOWN:
                HandleMouseClick(data, Event::Type::MouseButtonPressed, Mouse::Button::Left,
                                 lParam);
                break;
            case WM_LBUTTONUP:
                HandleMouseClick(data, Event::Type::MouseButtonReleased, Mouse::Button::Left,
                                 lParam);
                break;
            case WM_SIZING:
            {
                RECT* rect = reinterpret_cast<RECT*>(lParam);
                const Size size = {static_cast<Size::value_type>(rect->right - rect->left),
                                   static_cast<Size::value_type>(rect->bottom - rect->top)};
                ResizeEvent evt{ScaleSize(CalcAdjustedReverseWindowSize(size, data->style, data->hasMenu, data->scale.x * 96.0f),
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
                HandleWindowLimits(data, lParam);
                break;
            }
            case WM_DPICHANGED:
            {
                HandleDPIChange(data, wParam, lParam);
                break;
            }
            case WM_WINDOWPOSCHANGED:
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
                    HandleWindowResize(data, backend, hwnd);

                // Window was shown.
                if (!(winData->flags & SWP_SHOWWINDOW) && backend)
                    if (!backend->isMinimized() && data->minimized)
                        HandleWindowMinimize(data, false);

                // Window was hidden.
                if (!(winData->flags & SWP_HIDEWINDOW) && backend)
                    if (backend->isMinimized() && !data->minimized)
                        HandleWindowMinimize(data, true);

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
                    window->handleEvents();
                break;
            }
            case WM_COMMAND:
            {
                uint wmId{LOWORD(wParam)};
                Ref<MenuItem> item{MenuBarUtil_win::FindMenuItemById(data->menuItems, wmId)};

                if (item)
                {
                    NamedMenuItem *nItem = dynamic_cast<NamedMenuItem*>(item.get());
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
