//
//  platform/win/MainWindow_win.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "ptk/platform/mac/WindowHandle_mac.hpp"
#include "ptk/platform/common/RasterContext.hpp"
#include "ptk/platform/mac/ApplicationHandle_mac.hpp"
#include "ptk/platform/mac/RasterPolicy_mac.hpp"

// pTK Headers
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/events/KeyCodes.hpp"
#include "ptk/events/KeyMap.hpp"
#include "ptk/events/WindowEvent.hpp"
#include "ptk/Application.hpp"

// Include Metal backend.
#ifdef PTK_METAL
    #include "ptk/platform/mac/MetalContext_mac.hpp"
#endif

// macOS Headers
#include <Cocoa/Cocoa.h>

// C++ Headers
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace pTK
{
    // Since the iHandleEvent function is protected in WindowHandle, this is a friend function
    // to get around that issue. Maybe another way is better in the future, but this works
    // for now. This must be done since all macOS callbacks must be in a Objective-C interface.
    template<typename Event>
    void EventSendHelper(WindowHandle_mac* window, const Event& evt)
    {
        window->iHandleEvent<Event>(evt);
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface WindowDelegate : NSObject<NSWindowDelegate>
{
    pTK::WindowHandle_mac *ptkWindow;
}

- (WindowDelegate*)initWithWindow:(pTK::WindowHandle_mac*)pWindow;

@end

@implementation WindowDelegate

- (WindowDelegate*)initWithWindow:(pTK::WindowHandle_mac*)pWindow
{
    self = [super init];
    if (self != nil)
    {
         ptkWindow = pWindow;
    }

    return self;
}

- (BOOL)windowShouldClose:(NSWindow*) __unused sender
{
    pTK::CloseEvent evt{};
    pTK::EventSendHelper<pTK::CloseEvent>(ptkWindow, evt);

    return NO;
}

- (void)windowDidResize:(NSNotification*) __unused notification
{
    NSWindow *window = static_cast<NSWindow*>(ptkWindow->handle());
    const NSRect rect = [window.contentView bounds];
    const pTK::Size size{static_cast<pTK::Size::value_type>(rect.size.width),
                         static_cast<pTK::Size::value_type>(rect.size.height)};

    pTK::ResizeEvent evt{size};
    pTK::EventSendHelper<pTK::ResizeEvent>(ptkWindow, evt);
}

- (void)windowDidMove:(NSNotification*) __unused notification
{
    pTK::MoveEvent evt{ptkWindow->getWinPos()};
    pTK::EventSendHelper<pTK::MoveEvent>(ptkWindow, evt);
}

- (void)windowDidMiniaturize:(NSNotification*) __unused notification
{
    pTK::MinimizeEvent evt{};
    pTK::EventSendHelper<pTK::MinimizeEvent>(ptkWindow, evt);
}

- (void)windowDidDeminiaturize:(NSNotification*) __unused notification
{
    pTK::RestoreEvent evt{};
    pTK::EventSendHelper<pTK::RestoreEvent>(ptkWindow, evt);
}

- (void)windowDidBecomeKey:(NSNotification*) __unused notification
{
    pTK::FocusEvent evt{};
    pTK::EventSendHelper<pTK::FocusEvent>(ptkWindow, evt);
}

- (void)windowDidResignKey:(NSNotification*) __unused notification
{
    pTK::LostFocusEvent evt{};
    pTK::EventSendHelper<pTK::LostFocusEvent>(ptkWindow, evt);
}

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface MainView : NSView
{
    pTK::WindowHandle_mac *ptkWindow;
    std::underlying_type<pTK::KeyEvent::Modifier>::type modsPressed;
    NSTrackingArea *trackingArea;
}

- (MainView*)initWithWindow:(pTK::WindowHandle_mac*)initWindow;

@end

static std::underlying_type<pTK::KeyEvent::Modifier>::type GetModifiers(NSEventModifierFlags flags)
{
    using utype = std::underlying_type<pTK::KeyEvent::Modifier>::type;
    utype modifiers = 0;

    if (flags & NSEventModifierFlagShift) {
        modifiers |= static_cast<utype>(pTK::KeyEvent::Modifier::Shift);
    }

    if (flags & NSEventModifierFlagControl) {
        modifiers |= static_cast<utype>(pTK::KeyEvent::Modifier::Control);
    }

    if (flags & NSEventModifierFlagOption) {
        modifiers |= static_cast<utype>(pTK::KeyEvent::Modifier::Alt);
    }

    if (flags & NSEventModifierFlagCommand) {
        modifiers |= static_cast<utype>(pTK::KeyEvent::Modifier::Super);
    }

    if (flags & NSEventModifierFlagCapsLock) {
        modifiers |= static_cast<utype>(pTK::KeyEvent::Modifier::CapsLock);
    }

    return modifiers;
}

@implementation MainView

- (MainView*)initWithWindow:(pTK::WindowHandle_mac*)initWindow
{
    self = [super init];
    if (self != nil) {
        ptkWindow = initWindow;
        modsPressed = 0;

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
    pTK::PaintEvent evt{pTK::Point{0,0}, ptkWindow->getSize()};
    pTK::EventSendHelper<pTK::PaintEvent>(ptkWindow, evt);
}

- (void)drawRect:(NSRect) __unused rect
{
    pTK::PaintEvent evt{pTK::Point{0,0}, ptkWindow->getSize()};
    pTK::EventSendHelper<pTK::PaintEvent>(ptkWindow, evt);
}

static bool IsValid(uint32 data)
{
    bool valid{true};

    switch (data)
    {
        case 0x08: // Backspace
        case 0x0A: // Linefeed
        case 0x1B: // Escape
        case 0x09: // Tab
        case 0x0D: // Carriage return
        case 0x7F: // Delete
        case 0xF700: // Up arrow??
        case 0xF701: // Down arrow??
        case 0xF702: // Left arrow??
        case 0xF703: // Right arrow??
        {
            valid = false;
            break;
        }
        default:
            break;
    }

    return valid;
}

- (void)keyDown:(NSEvent *)event
{
    std::underlying_type<pTK::KeyEvent::Modifier>::type mods{GetModifiers(event.modifierFlags)};
    uint32 data{0};

    pTK::KeyEvent press{pTK::Event::Type::KeyPressed, pTK::KeyMap::KeyCodeToKey(static_cast<byte>(event.keyCode)),
        data, pTK::Text::Encoding::UTF32, mods};
    pTK::EventSendHelper<pTK::KeyEvent>(ptkWindow, press);

    if ([event.characters canBeConvertedToEncoding:NSUTF32StringEncoding])
    {
        NSData *utf32Data = [event.characters dataUsingEncoding:NSUTF32StringEncoding];
        const uint32 *utf32 = static_cast<const uint32*>([utf32Data bytes]);
        NSUInteger count = [utf32Data length] / 4;

        if (count > 0)
        {
            std::size_t validCount{0};
            for (std::size_t i{1}; i < count; ++i)
                if (IsValid(utf32[i]))
                    ++validCount;

            if (validCount > 0)
            {
                pTK::InputEvent::data_cont arr(new pTK::InputEvent::data_type[validCount]);
                std::size_t index{0};
                for (std::size_t i{1}; i < count; ++i)
                    if (IsValid(utf32[i]))
                        arr[index++] = utf32[i];

                // Trigger event.
                pTK::InputEvent input{arr, validCount, pTK::Text::Encoding::UTF32};
                pTK::EventSendHelper<pTK::InputEvent>(ptkWindow, input);
            }
        }
    }

    // [self interpretKeyEvents:@[event]];
}

- (void)keyUp:(NSEvent *)event
{
    std::underlying_type<pTK::KeyEvent::Modifier>::type mods{GetModifiers(event.modifierFlags)};
    pTK::KeyEvent evt{pTK::Event::Type::KeyReleased, pTK::KeyMap::KeyCodeToKey(static_cast<byte>(event.keyCode)), mods};
    pTK::EventSendHelper<pTK::KeyEvent>(ptkWindow, evt);
}

- (void)flagsChanged:(NSEvent *)event
{
    pTK::KeyCode key = pTK::KeyMap::KeyCodeToKey(static_cast<byte>(event.keyCode));
    pTK::KeyEvent::Modifier keyMod = pTK::KeyCodeToKeyEventModifier(key);

    using utype = std::underlying_type<pTK::KeyEvent::Modifier>::type;
    utype keyFlagMod = static_cast<utype>(keyMod);

    std::underlying_type<pTK::KeyEvent::Modifier>::type mods{GetModifiers([event modifierFlags])};
    pTK::Event::Type type = pTK::Event::Type::KeyReleased;

    if (keyFlagMod != 0)
    {
        if (modsPressed & keyFlagMod)
            modsPressed &= ~(keyFlagMod);
        else
        {
            type = pTK::Event::Type::KeyPressed;
            modsPressed |= keyFlagMod;
        }
    }

    pTK::KeyEvent evt{type, key, mods};
    pTK::EventSendHelper<pTK::KeyEvent>(ptkWindow, evt);
}

- (BOOL)acceptsFirstMouse:(NSEvent*) __unused event
{
    return YES;
}

- (void)mouseDown:(NSEvent*) __unused event
{
    NSWindow *nswindow = static_cast<NSWindow*>(ptkWindow->handle());
    const NSPoint pos = [nswindow mouseLocationOutsideOfEventStream];
    const NSRect content = [nswindow.contentView frame];
    pTK::ClickEvent evt{pTK::Mouse::Button::Left,
                         {static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    pTK::EventSendHelper<pTK::ClickEvent>(ptkWindow, evt);
}

- (void)mouseUp:(NSEvent*) __unused event
{
    NSWindow *nswindow = static_cast<NSWindow*>(ptkWindow->handle());
    const NSPoint pos = [nswindow mouseLocationOutsideOfEventStream];
    const NSRect content = [nswindow.contentView frame];
    pTK::ReleaseEvent evt{pTK::Mouse::Button::Left,
                         {static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    pTK::EventSendHelper<pTK::ReleaseEvent>(ptkWindow, evt);
}

- (void)mouseMoved:(NSEvent*) event
{
    NSWindow *nswindow = static_cast<NSWindow*>(ptkWindow->handle());
    const NSPoint pos = [event locationInWindow];
    const NSRect content = [nswindow.contentView frame];
    pTK::MotionEvent evt{{static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    pTK::EventSendHelper<pTK::MotionEvent>(ptkWindow, evt);
}

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace pTK
{
    static std::unique_ptr<ContextBase> CreateMacContext(WindowInfo::Backend type, NSWindow *nswindow, const Size& size, const Vec2f& scale)
    {
#ifdef PTK_DEBUG
      if (scale.x != scale.y)
          PTK_WARN("Window scale is not the same, x: {}, y: {}", static_cast<double>(scale.x), static_cast<double>(scale.y));
#endif
        const Size scaledSize{static_cast<Size::value_type>(size.width * scale.x),
                            static_cast<Size::value_type>(size.height * scale.y)};
#ifdef PTK_METAL
        if (type == WindowInfo::Backend::Hardware)
            return std::make_unique<MetalContext_mac>(static_cast<void*>([nswindow contentView]), scaledSize, scale);
#endif

        // Software backend is always available.
        // Altough, it is slow on macOS and should be avoided.
        PTK_WARN("Software rendering on macOS is slow and should not be used");
        RasterPolicy_mac policy(nswindow);
        return std::make_unique<RasterContext<RasterPolicy_mac>>(scaledSize, policy);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    WindowHandle_mac::WindowHandle_mac(const std::string& name, const Size& size, const WindowInfo& flags)
        : WindowHandle(size, flags)
    {
        init(name, size, flags);
    }

    WindowHandle_mac::~WindowHandle_mac()
    {
        close();
        PTK_INFO("Destroyed WindowHandle_mac");
    }

    void WindowHandle_mac::init(const std::string& name, const Size& size, const WindowInfo& flags)
    {
        @autoreleasepool {
            WindowDelegate *winDelegate = [[WindowDelegate alloc] initWithWindow:this];

            // Default is currently MainDisplay (where 0,0 is), could cause some headaches later on.
            // Same with setPosHint, don't have a secondary monitor to test on right now.
            const auto ypos = static_cast<Size::value_type>(flags.position.y);
            const CGFloat y = CGDisplayBounds(CGMainDisplayID()).size.height - (ypos + size.height - 1) - 1;
            const NSRect rect = NSMakeRect(flags.position.x, y, size.width, size.height);

            NSUInteger style = (NSWindowStyleMaskTitled | NSWindowStyleMaskClosable | NSWindowStyleMaskResizable | NSWindowStyleMaskMiniaturizable);
            NSWindow *nswindow = [[NSWindow alloc] initWithContentRect:rect styleMask:style backing:NSBackingStoreBuffered defer:NO];
            m_NSWindow = static_cast<void*>(nswindow);

            MainView *view = [[MainView alloc] initWithWindow:this];
            if (view == nil)
                throw WindowError("Failed to allocate Mainview");

            [nswindow setContentView:view];
            [nswindow makeFirstResponder:view];
            [nswindow setDelegate:winDelegate];
            [nswindow setAcceptsMouseMovedEvents:YES];
            [nswindow setRestorable:NO];

            [view release];

            m_id = nswindow.windowNumber;

            const NSRect fRect = [nswindow.contentView frame];
            const NSRect pRect = [nswindow.contentView convertRectToBacking:fRect];
            m_scale = {static_cast<Vec2f::value_type>(pRect.size.width / fRect.size.width),
                             static_cast<Vec2f::value_type>(pRect.size.height / fRect.size.height)};
            PTK_INFO("System DPI scale is {0:0.2f}x{1:0.2f}", static_cast<double>(m_scale.x), static_cast<double>(m_scale.y));

            m_context = CreateMacContext(flags.backend, nswindow, size, m_scale);

            switch (flags.visibility)
            {
                case WindowInfo::Visibility::Windowed:
                    show();
                    break;
                case WindowInfo::Visibility::Hidden: // Window is started hidden.
                    break;
            }

            setTitle(name);
            ApplicationHandle_mac::SetMenuBar(flags.menus);
        }
        PTK_INFO("Initialized WindowHandle_mac");
    }

    void WindowHandle_mac::setPosHint(const Point& pos)
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            const NSRect content = [nswindow.contentView frame];
            const CGFloat y = CGDisplayBounds(CGMainDisplayID()).size.height - (pos.y + content.size.height - 1) - 1;
            const NSRect rect = NSMakeRect(pos.x, y, 0, 0);
            const NSRect frame = [nswindow frameRectForContentRect:rect];
            [nswindow setFrameOrigin:frame.origin];
        }
    }

    bool WindowHandle_mac::setTitle(const std::string& name)
    {
        NSString *str = [NSString stringWithUTF8String:name.c_str()];
        if (!str)
            return false;

        NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
        [nswindow setTitle:str];
        return true;
    }

    bool WindowHandle_mac::setIcon(int32, int32, byte*)
    {
        PTK_WARN("macOS Windows does not have icons");
        return false;
    }

    void WindowHandle_mac::notifyEvent()
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

    void WindowHandle_mac::swapBuffers()
    {
        m_context->swapBuffers();
    }

    bool WindowHandle_mac::resize(const Size& size)
    {
        @autoreleasepool {
            bool ret{false};

            if (size != getWinSize())
            {
                PTK_INFO("bool WindowHandle_mac::resize(const Size& size)");
                NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
                NSRect content = [nswindow contentRectForFrameRect:[nswindow frame]];
                content.origin.y += content.size.height - size.height;
                content.size = NSMakeSize(size.width, size.height);
                [nswindow setFrame:[nswindow frameRectForContentRect:content] display:YES];
                ret = true;
            }

            const Size scaledSize{static_cast<Size::value_type>(size.width * m_scale.x),
                static_cast<Size::value_type>(size.height * m_scale.y)};
            if (scaledSize != m_context->getSize())
            {
                m_context->resize(scaledSize);
                ret = true;
            }

            return ret;
        }
    }

    bool WindowHandle_mac::close()
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            if (nswindow != nil)
            {
                [nswindow close];
                nswindow = nil;
                m_NSWindow = nullptr;
                if (auto win = dynamic_cast<Window*>(this))
                    Application::Get()->removeWindow(win);
            }

            PTK_INFO("bool WindowHandle_mac::close()");
            return true;
        }
    }

    void WindowHandle_mac::show()
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            if (nswindow != nil)
            {
                [nswindow orderFront:nil];
                [NSApp activateIgnoringOtherApps:YES];
                [nswindow makeKeyAndOrderFront:NSApp];
            }
        }
    }

    void WindowHandle_mac::hide()
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            if (nswindow != nil)
                [nswindow orderOut:nil];
        }
    }

    bool WindowHandle_mac::isHidden() const
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            return ![nswindow isVisible];
        }
    }

    ContextBase* WindowHandle_mac::getContext() const
    {
        return m_context.get();
    }

    Vec2f WindowHandle_mac::getDPIScale() const
    {
        return m_scale;
    }

    Point WindowHandle_mac::getWinPos() const
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            const NSRect rect = [nswindow contentRectForFrameRect:[nswindow frame]];
            using pType = pTK::Point::value_type;
            const pType y{static_cast<pType>(CGDisplayBounds(CGMainDisplayID()).size.height - (rect.origin.y + rect.size.height - 1) - 1)};
            return Point{static_cast<pType>(rect.origin.x), y};
        }
    }

    Size WindowHandle_mac::getWinSize() const
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            const NSRect rect = [[nswindow contentView] frame];
            return {static_cast<Size::value_type>(rect.size.width),
                    static_cast<Size::value_type>(rect.size.height)};
        }
    }


    void WindowHandle_mac::setWindowLimits(const Size& min, const Size& max)
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            CGFloat frameHeight = nswindow.frame.size.height - [nswindow contentRectForFrameRect: nswindow.frame].size.height;

            const NSSize minSize = NSMakeSize(min.width, min.height + frameHeight);
            [nswindow setMinSize:minSize];

            const NSSize maxSize = NSMakeSize(max.width, max.height + ((max.height != Size::Limits::Max) ? frameHeight : 0));
            [nswindow setMaxSize:maxSize];
            PTK_INFO("Updated window limits to {}x{} and {}x{}", min.width, min.height, max.width, max.height);
        }
    }


    bool WindowHandle_mac::minimize()
    {
        @autoreleasepool {
            PTK_INFO("bool WindowHandle_mac::minimize()");
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            [nswindow miniaturize:nil];
            return true;
        }
    }

    bool WindowHandle_mac::isMinimized() const
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            return [nswindow isMiniaturized];;
        }
    }

    bool WindowHandle_mac::restore()
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            [nswindow deminiaturize:nil];
            return true;
        }
    }

    bool WindowHandle_mac::isFocused() const
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            return [nswindow isKeyWindow];
        }
    }

    bool WindowHandle_mac::setScaleHint(const Vec2f& scale)
    {
        if (m_scale != scale)
        {
            m_scale = scale;
            resize(getSize());
            return true;
        }
        return false;
    }

    long WindowHandle_mac::windowID() const
    {
        return m_id;
    }

    void *WindowHandle_mac::handle() const
    {
        return m_NSWindow;
    }

} // namespace pTK
