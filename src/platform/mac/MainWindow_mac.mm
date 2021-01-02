//
//  platform/win/MainWindow_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "MainWindow_mac.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/events/KeyCodes.hpp"
#include "ptk/events/WindowEvent.hpp"
#include "ptk/Application.hpp"

#include "../common/RasterContext.hpp"
#include "Application_mac.hpp"
#include "RasterPolicy_mac.hpp"

// Include Metal backend.
#ifdef PTK_METAL
    #include "MetalContext_mac.hpp"
#endif

// macOS Headers
#include <Cocoa/Cocoa.h>

// C++ Headers
#include <cmath>

namespace pTK
{
    static std::map<byte, KeyCode> InitKeyCodes() noexcept
    {
        std::map<byte, KeyCode> map{};
        map[0x31] = Key::Space; map[0x35] = Key::Escape;
        map[0x1D] = Key::D0; map[0x12] = Key::D1; map[0x13] = Key::D2; map[0x14] = Key::D3;
        map[0x15] = Key::D4; map[0x17] = Key::D5; map[0x16] = Key::D6; map[0x1A] = Key::D7;
        map[0x1C] = Key::D8; map[0x19] = Key::D9;
        map[0x52] = Key::D0; map[0x53] = Key::D1; map[0x54] = Key::D2; map[0x55] = Key::D3;
        map[0x56] = Key::D4; map[0x57] = Key::D5; map[0x58] = Key::D6; map[0x59] = Key::D7;
        map[0x5B] = Key::D8; map[0x5C] = Key::D9;
        map[0x00] = Key::A; map[0x0B] = Key::B; map[0x08] = Key::C; map[0x02] = Key::D;
        map[0x0E] = Key::E; map[0x03] = Key::F; map[0x05] = Key::G; map[0x04] = Key::H;
        map[0x22] = Key::I; map[0x26] = Key::J; map[0x28] = Key::K; map[0x25] = Key::L;
        map[0x2E] = Key::M; map[0x2D] = Key::N; map[0x1F] = Key::O; map[0x23] = Key::P;
        map[0x0C] = Key::Q; map[0x0F] = Key::R; map[0x01] = Key::S; map[0x11] = Key::T;
        map[0x20] = Key::U; map[0x09] = Key::V; map[0x0D] = Key::W; map[0x07] = Key::X;
        map[0x10] = Key::Y; map[0x06] = Key::Z;
        return map;
    }

    static const std::map<byte, KeyCode> s_keyMap{InitKeyCodes()};

    static Key TranslateKeyCodeToKey(byte code)
    {
        std::map<byte, KeyCode>::const_iterator it{s_keyMap.find(code)};
        if (it != s_keyMap.cend())
            return it->second;

        return Key::Unknown;
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////

    struct MainWindow_mac::WinData
    {
        NSWindow *window;
        Size size;
        Point pos;
        Vec2f scale;
        long id;
    };

} // namespace pTK

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface WindowDelegate : NSObject<NSWindowDelegate>
{
    pTK::MainWindow_mac *ptkWindow;
    pTK::MainWindow_mac::WinData *data;
}

- (WindowDelegate*)initWithWindow:(pTK::MainWindow_mac*)pWindow :(pTK::MainWindow_mac::WinData*)winData;

@end

@implementation WindowDelegate

- (WindowDelegate*)initWithWindow:(pTK::MainWindow_mac*)pWindow :(pTK::MainWindow_mac::WinData*)winData
{
    self = [super init];
    if (self != nil)
    {
         ptkWindow = pWindow;
         data = winData;
    }

    return self;
}

- (BOOL)windowShouldClose:(NSWindow*) __unused sender
{
    pTK::Event evt{pTK::Event::Category::Window, pTK::Event::Type::WindowClose};
    ptkWindow->parent()->handleEvents(); // Handle all events before sending close event.
    ptkWindow->parent()->sendEvent(&evt);

    return NO;
}

- (void)windowDidResize:(NSNotification*) __unused notification
{
    const NSRect rect = [data->window.contentView bounds];
    const pTK::Size size{static_cast<pTK::Size::value_type>(rect.size.width),
                         static_cast<pTK::Size::value_type>(rect.size.height)};
    if (size != data->size)
    {
        pTK::ResizeEvent evt{size};
        ptkWindow->parent()->sendEvent(&evt);
        ptkWindow->parent()->forceDrawAll();
    }

}

- (void)windowDidMove:(NSNotification*) __unused notification
{
    pTK::Point pos{ptkWindow->getWinPos()};
    if (pos != data->pos)
    {
        pTK::MoveEvent evt{pos};
        ptkWindow->parent()->sendEvent(&evt);
    }
}

- (void)windowDidMiniaturize:(NSNotification*) __unused notification
{
    pTK::Event evt{pTK::Event::Category::Window, pTK::Event::Type::WindowMinimize};
    ptkWindow->parent()->sendEvent(&evt);
}

- (void)windowDidDeminiaturize:(NSNotification*) __unused notification
{
    pTK::Event evt{pTK::Event::Category::Window, pTK::Event::Type::WindowRestore};
    ptkWindow->parent()->sendEvent(&evt);
}

- (void)windowDidBecomeKey:(NSNotification*) __unused notification
{
    pTK::Event evt{pTK::Event::Category::Window, pTK::Event::Type::WindowFocus};
    ptkWindow->parent()->sendEvent(&evt);
}

- (void)windowDidResignKey:(NSNotification*) __unused notification
{
    pTK::Event evt{pTK::Event::Category::Window, pTK::Event::Type::WindowLostFocus};
    ptkWindow->parent()->sendEvent(&evt);
}

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface MainView : NSView
{
    pTK::MainWindow_mac *window;
    pTK::MainWindow_mac::WinData *data;
    NSTrackingArea *trackingArea;
}

- (MainView*)initWithWindow:(pTK::MainWindow_mac*)initWindow :(pTK::MainWindow_mac::WinData*)winData;

@end

@implementation MainView

- (MainView*)initWithWindow:(pTK::MainWindow_mac*)initWindow :(pTK::MainWindow_mac::WinData*)winData
{
    self = [super init];
    if (self != nil) {
        window = initWindow;
        data = winData;

        const NSTrackingAreaOptions options = NSTrackingMouseEnteredAndExited | NSTrackingActiveInKeyWindow |
            NSTrackingEnabledDuringMouseDrag | NSTrackingCursorUpdate | NSTrackingInVisibleRect | NSTrackingAssumeInside;
        trackingArea = [[NSTrackingArea alloc] initWithRect:[self bounds]
                        options:options
                        owner:self
                        userInfo:nil];
        [self addTrackingArea:trackingArea];
    }

    return self;
}

- (void)dealloc
{
    [trackingArea release];
    [super dealloc];
}


- (BOOL)isOpaque
{
    return YES;
}

- (BOOL)canBecomeKeyView
{
    return YES;
}

- (BOOL)acceptsFirstResponder
{
    return YES;
}

- (BOOL)wantsUpdateLayer
{
    return YES;
}

- (void)updateLayer
{
    pTK::PaintEvent evt{pTK::Point{0,0}, window->parent()->getSize()};
    window->parent()->sendEvent(&evt);
}

- (void)drawRect:(NSRect) __unused rect
{
    pTK::PaintEvent evt{pTK::Point{0,0}, window->parent()->getSize()};
    window->parent()->sendEvent(&evt);
}


- (void)keyDown:(NSEvent *)event
{
    const int code = event.keyCode;
    pTK::KeyEvent evt{pTK::Event::Type::KeyPressed, pTK::TranslateKeyCodeToKey(static_cast<byte>(code))};
    window->parent()->sendEvent(&evt);
    
    [self interpretKeyEvents:@[event]];
}

- (void)keyUp:(NSEvent *)event
{
    const int code = event.keyCode;
    pTK::KeyEvent evt{pTK::Event::Type::KeyReleased, pTK::TranslateKeyCodeToKey(static_cast<byte>(code))};
    window->parent()->sendEvent(&evt);
}

- (BOOL)acceptsFirstMouse:(NSEvent*) __unused event
{
    return YES;
}

- (void)mouseDown:(NSEvent*) __unused event
{
    const NSPoint pos = [data->window mouseLocationOutsideOfEventStream];
    const NSRect content = [data->window.contentView frame];
    pTK::ButtonEvent evt{pTK::Event::Type::MouseButtonPressed,
                         pTK::Mouse::Button::Left,
                         {static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    window->parent()->sendEvent(&evt);
}

- (void)mouseUp:(NSEvent*) __unused event
{
    const NSPoint pos = [data->window mouseLocationOutsideOfEventStream];
    const NSRect content = [data->window.contentView frame];
    pTK::ButtonEvent evt{pTK::Event::Type::MouseButtonReleased,
                         pTK::Mouse::Button::Left,
                         {static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    window->parent()->sendEvent(&evt);
}

- (void)mouseMoved:(NSEvent*) event
{
    const NSPoint pos = [event locationInWindow];
    const NSRect content = [data->window.contentView frame];
    pTK::MotionEvent evt{{static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    window->parent()->sendEvent(&evt);
}

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace pTK
{
    static std::unique_ptr<ContextBase> createMacContext(WindowInfo::Backend type, const std::unique_ptr<MainWindow_mac::WinData>& data)
    {
#ifdef PTK_DEBUG
      if (data->scale.x != data->scale.y)
          PTK_WARN("Window scale is not the same, x: {}, y: {}", data->scale.x, data->scale.y);
#endif
        const Size scaledSize{static_cast<Size::value_type>(data->size.width * data->scale.x),
                            static_cast<Size::value_type>(data->size.height * data->scale.y)};
#ifdef PTK_METAL
        if (type == WindowInfo::Backend::Hardware)
            return std::make_unique<MetalContext_mac>(static_cast<void*>([data->window contentView]), scaledSize, data->scale);
#endif

        // Software backend is always available.
        // Altough, it is slow on macOS and should be avoided.
        RasterPolicy_mac policy(data->window);
        return std::make_unique<RasterContext<RasterPolicy_mac>>(scaledSize, policy);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    MainWindow_mac::MainWindow_mac(Window *window, const std::string& name, const Size& size, WindowInfo info)
        : MainWindowBase(window)
    {
        init(name, size, info);
    }

    MainWindow_mac::~MainWindow_mac()
    {
        if (m_data->window != nil)
        {
            [m_data->window close];
            m_data->window = nil;
        }
        PTK_INFO("Destroyed MainWindow_mac");
    }

    void MainWindow_mac::init(const std::string& name, const Size& size, WindowInfo info)
    {
        try {
            m_data = std::make_unique<MainWindow_mac::WinData>();
        } catch (const std::bad_alloc&) {
            throw WindowError("Failed to allocate memory for MainWindow_mac");
        }

        m_data->size = size;
        m_data->pos = {};

        @autoreleasepool {
            WindowDelegate *winDelegate = [[WindowDelegate alloc] initWithWindow:this:m_data.get()];

            NSRect rect = NSMakeRect(100, 100, size.width, size.height);
            NSUInteger style = (NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable);
            m_data->window = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:NO];

            MainView *view = [[MainView alloc] initWithWindow:this:m_data.get()];
            if (view == nil)
                throw WindowError("Failed to allocate Mainview");

            [m_data->window setContentView:view];
            [m_data->window makeFirstResponder:view];
            [m_data->window setDelegate:winDelegate];
            [m_data->window setAcceptsMouseMovedEvents:YES];
            [m_data->window setRestorable:NO];

            [view release];

            m_data->id = m_data->window.windowNumber;

            const NSRect fRect = [m_data->window.contentView frame];
            const NSRect pRect = [m_data->window.contentView convertRectToBacking:fRect];
            m_data->scale = {static_cast<Vec2f::value_type>(pRect.size.width / fRect.size.width),
                             static_cast<Vec2f::value_type>(pRect.size.height / fRect.size.height)};
            PTK_INFO("System DPI scale is {0:0.2f}x{1:0.2f}", static_cast<double>(m_data->scale.x), static_cast<double>(m_data->scale.y));

            setTitle(name);
            m_data->pos = getWinPos();

            m_context = createMacContext(info.backend, m_data);
            
            switch (info.visibility) {
                case WindowInfo::Visibility::Windowed:
                    show();
                    break;
                case WindowInfo::Visibility::Hidden: // Window is started hidden.
                    break;
                    
                default:
                    break;
            }

        }
        PTK_INFO("Initialized MainWindow_mac");
    }

    bool MainWindow_mac::setPosHint(const Point& pos)
    {
        if (pos != m_data->pos)
        {
            @autoreleasepool {
                const NSRect content = [m_data->window.contentView frame];
                const CGFloat y = CGDisplayBounds(CGMainDisplayID()).size.height - (pos.y + content.size.height - 1) - 1;
                const NSRect rect = NSMakeRect(pos.x, y, 0, 0);
                const NSRect frame = [m_data->window frameRectForContentRect:rect];
                [m_data->window setFrameOrigin:frame.origin];
                m_data->pos = pos;
                return true;
            }
        }
        return false;
    }

    bool MainWindow_mac::setTitle(const std::string& name)
    {
        NSString *str = [NSString stringWithUTF8String:name.c_str()];
        if (!str)
            return false;

        [m_data->window setTitle:str];
        return true;
    }

    bool MainWindow_mac::setIcon(int32, int32, byte*)
    {
        PTK_WARN("macOS Windows does not have icons");
        return false;
    }

    void MainWindow_mac::notifyEvent()
    {
        @autoreleasepool {
            NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                                location:NSMakePoint(0, 0)
                                           modifierFlags:0
                                               timestamp:0
                                            windowNumber:0
                                                 context:nil
                                                 subtype:0
                                                   data1:0
                                                   data2:0];
            dispatch_async(dispatch_get_main_queue(), ^{
                [NSApp postEvent:event atStart:YES];
            });
        }
    }

    void MainWindow_mac::swapBuffers()
    {
        m_context->swapBuffers();
    }

    bool MainWindow_mac::resize(const Size& size)
    {
        if (size != m_data->size)
        {
            @autoreleasepool {
                NSRect content = [m_data->window contentRectForFrameRect:[m_data->window frame]];
                content.origin.y += content.size.height - size.height;
                content.size = NSMakeSize(size.width, size.height);
                [m_data->window setFrame:[m_data->window frameRectForContentRect:content] display:YES];
                const Size scaledSize{static_cast<Size::value_type>(size.width * m_data->scale.x),
                                      static_cast<Size::value_type>(size.height * m_data->scale.y)};
                if (scaledSize != m_context->getSize())
                    m_context->resize(scaledSize);
                m_data->size = size;
                return true;
            }
        }
        return false;
    }

    bool MainWindow_mac::close()
    {
        @autoreleasepool {
            [m_data->window close];
            m_data->window = nil;
            Application::Get()->removeWindow(parent());
            return true;
        }
    }

    bool MainWindow_mac::show()
    {
        @autoreleasepool {
            if (m_data->window != nil)
            {
                [m_data->window orderFront:nil];
                [NSApp activateIgnoringOtherApps:YES];
                [m_data->window makeKeyAndOrderFront:NSApp];
                return true;
            }
            return false;
        }
    }

    bool MainWindow_mac::hide()
    {
        @autoreleasepool {
            if (m_data->window != nil)
            {
                [m_data->window orderOut:nil];
                return true;
            }
            return false;
        }
    }

    bool MainWindow_mac::isHidden() const
    {
        @autoreleasepool {
            return [m_data->window isVisible];;
        }
    }

    ContextBase* MainWindow_mac::getContext() const
    {
        return m_context.get();
    }

    Vec2f MainWindow_mac::getDPIScale() const
    {
        return m_data->scale;
    }

    Point MainWindow_mac::getWinPos() const
    {
        @autoreleasepool {
            const NSRect rect = [m_data->window contentRectForFrameRect:[m_data->window frame]];
            using pType = pTK::Point::value_type;
            const pType y{static_cast<pType>(CGDisplayBounds(CGMainDisplayID()).size.height - (rect.origin.y + rect.size.height - 1) - 1)};
            return Point{static_cast<pType>(rect.origin.x), y};
        }
    }

    Size MainWindow_mac::getWinSize() const
    {
        @autoreleasepool {
            const NSRect rect = [[m_data->window contentView] frame];
            return {static_cast<Size::value_type>(rect.size.width),
                    static_cast<Size::value_type>(rect.size.height)};
        }
    }

    bool MainWindow_mac::setLimits(const Size&, const Size&)
    {
        // TODO
        return false;
    }

    bool MainWindow_mac::minimize()
    {
        @autoreleasepool {
            [m_data->window miniaturize:nil];
            return true;
        }
    }

    bool MainWindow_mac::isMinimized() const
    {
        @autoreleasepool {
            return [m_data->window isMiniaturized];;
        }
    }

    bool MainWindow_mac::restore()
    {
        @autoreleasepool {
            [m_data->window deminiaturize:nil];
            return true;
        }
    }

    bool MainWindow_mac::isFocused() const
    {
        @autoreleasepool {
            return [m_data->window isKeyWindow];
        }
    }

    bool MainWindow_mac::setScaleHint(const Vec2f& scale)
    {
        if (m_data->scale != scale)
        {
            m_data->scale = scale;
            resize(parent()->getSize());
            return true;
        }
        return false;
    }

    long MainWindow_mac::windowID() const
    {
        return m_data->id;
    }

    void *MainWindow_mac::nsWindow() const
    {
        return static_cast<void*>(m_data->window);
    }

} // namespace pTK
