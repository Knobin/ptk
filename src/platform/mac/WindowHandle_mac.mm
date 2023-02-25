//
//  src/platform/mac/WindowHandle_mac.mm
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-07.
//

// Local Headers
#include "WindowHandle_mac.hpp"
#include "../../Log.hpp"
#include "ApplicationHandle_mac.hpp"

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/events/KeyCodes.hpp"
#include "ptk/events/KeyMap.hpp"
#include "ptk/events/MouseEvent.hpp"
#include "ptk/events/WindowEvent.hpp"

// macOS Headers
#include <Cocoa/Cocoa.h>

// C++ Headers
#include <cmath>

namespace pTK::Platform::WindowHandleFactoryImpl
{
    std::unique_ptr<WindowHandle> Make(WindowBase* base, const std::string& name, const Size& size,
                                       const WindowInfo& info)
    {
        return std::make_unique<WindowHandle_mac>(base, name, size, info);
    }
} // namespace pTK::Platform::WindowHandleFactoryImpl

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface WindowDelegate : NSObject<NSWindowDelegate>
{
    pTK::Platform::WindowHandle_mac* ptkWindow;
}

- (WindowDelegate*)initWithWindow:(pTK::Platform::WindowHandle_mac*)pWindow;

@end

@implementation WindowDelegate

- (WindowDelegate*)initWithWindow:(pTK::Platform::WindowHandle_mac*)pWindow
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
    ptkWindow->HandlePlatformEvent<pTK::CloseEvent>(evt);
    return NO;
}

- (void)windowDidResize:(NSNotification*) __unused notification
{
    NSWindow* window = static_cast<NSWindow*>(ptkWindow->nswindow());
    const NSRect rect = [window.contentView bounds];
    const pTK::Size size{static_cast<pTK::Size::value_type>(rect.size.width),
                         static_cast<pTK::Size::value_type>(rect.size.height)};

    pTK::ResizeEvent rEvt{size};
    ptkWindow->HandlePlatformEvent<pTK::ResizeEvent>(rEvt);

    // Window did not paint until resize was fully completed.
    // This will ensure that it will paint after every size change.
    // TODO(knobin): This might be macOS specific in this implementation, if not this can be moved to
    // WindowBase handler for ResizeEvent.
    pTK::PaintEvent pEvt{pTK::Point{0, 0}, ptkWindow->getSize()};
    ptkWindow->HandlePlatformEvent<pTK::PaintEvent>(pEvt);
}

- (void)windowDidMove:(NSNotification*) __unused notification
{
    pTK::MoveEvent evt{ptkWindow->getPosition()};
    ptkWindow->HandlePlatformEvent<pTK::MoveEvent>(evt);
}

- (void)windowDidMiniaturize:(NSNotification*) __unused notification
{
    pTK::MinimizeEvent evt{};
    ptkWindow->HandlePlatformEvent<pTK::MinimizeEvent>(evt);
}

- (void)windowDidDeminiaturize:(NSNotification*) __unused notification
{
    pTK::RestoreEvent evt{};
    ptkWindow->HandlePlatformEvent<pTK::RestoreEvent>(evt);
}

- (void)windowDidBecomeKey:(NSNotification*) __unused notification
{
    pTK::FocusEvent evt{};
    ptkWindow->HandlePlatformEvent<pTK::FocusEvent>(evt);
}

- (void)windowDidResignKey:(NSNotification*) __unused notification
{
    pTK::LostFocusEvent evt{};
    ptkWindow->HandlePlatformEvent<pTK::LostFocusEvent>(evt);
}

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

@interface MainView : NSView
{
    pTK::Platform::WindowHandle_mac* ptkWindow;
    std::underlying_type<pTK::KeyEvent::Modifier>::type modsPressed;
    NSTrackingArea *trackingArea;
}

- (MainView*)initWithWindow:(pTK::Platform::WindowHandle_mac*)initWindow;

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

- (MainView*)initWithWindow:(pTK::Platform::WindowHandle_mac*)initWindow
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
    ptkWindow->HandlePlatformEvent<pTK::PaintEvent>(evt);
}

- (void)drawRect:(NSRect) __unused rect
{
    pTK::PaintEvent evt{pTK::Point{0,0}, ptkWindow->getSize()};
    ptkWindow->HandlePlatformEvent<pTK::PaintEvent>(evt);
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

    pTK::KeyEvent press{pTK::Event::Type::KeyPressed, pTK::KeyMap::KeyCodeToKey(static_cast<uint8_t>(event.keyCode)),
        data, pTK::Text::Encoding::UTF32, mods};
    ptkWindow->HandlePlatformEvent<pTK::KeyEvent>(press);

    if ([event.characters canBeConvertedToEncoding:NSUTF32StringEncoding])
    {
        NSData *utf32Data = [event.characters dataUsingEncoding:NSUTF32StringEncoding];
        const auto *utf32 = static_cast<const uint32_t*>([utf32Data bytes]);
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
                ptkWindow->HandlePlatformEvent<pTK::InputEvent>(input);
            }
        }
    }

    // [self interpretKeyEvents:@[event]];
}

- (void)keyUp:(NSEvent *)event
{
    std::underlying_type<pTK::KeyEvent::Modifier>::type mods{GetModifiers(event.modifierFlags)};
    pTK::KeyEvent evt{pTK::Event::Type::KeyReleased, pTK::KeyMap::KeyCodeToKey(static_cast<uint8_t>(event.keyCode)), mods};
    ptkWindow->HandlePlatformEvent<pTK::KeyEvent>(evt);
}

- (void)scrollWheel:(NSEvent *)event
{
    const double delta{([event hasPreciseScrollingDeltas]) ? 0.1 : 1.0};
    const pTK::ScrollEvent scrollEvent{
        {static_cast<float>([event scrollingDeltaX] * delta), static_cast<float>([event scrollingDeltaY] * delta)}};

    if (std::fabs(scrollEvent.offset.x) > 0.0f || std::fabs(scrollEvent.offset.y) > 0.0f)
        ptkWindow->HandlePlatformEvent<pTK::ScrollEvent>(scrollEvent);
}

- (void)flagsChanged:(NSEvent *)event
{
    pTK::KeyCode key = pTK::KeyMap::KeyCodeToKey(static_cast<uint8_t>(event.keyCode));
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
    ptkWindow->HandlePlatformEvent<pTK::KeyEvent>(evt);
}

- (BOOL)acceptsFirstMouse:(NSEvent*) __unused event
{
    return YES;
}

- (void)mouseDown:(NSEvent*) __unused event
{
    NSWindow* nswindow = static_cast<NSWindow*>(ptkWindow->nswindow());
    const NSPoint pos = [nswindow mouseLocationOutsideOfEventStream];
    const NSRect content = [nswindow.contentView frame];
    pTK::ClickEvent evt{pTK::Mouse::Button::Left, -1,
                         {static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    ptkWindow->HandlePlatformEvent<pTK::ClickEvent>(evt);
}

- (void)mouseUp:(NSEvent*) __unused event
{
    NSWindow* nswindow = static_cast<NSWindow*>(ptkWindow->nswindow());
    const NSPoint pos = [nswindow mouseLocationOutsideOfEventStream];
    const NSRect content = [nswindow.contentView frame];
    pTK::ReleaseEvent evt{pTK::Mouse::Button::Left, -1,
                         {static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    ptkWindow->HandlePlatformEvent<pTK::ReleaseEvent>(evt);
}

- (void)mouseMoved:(NSEvent*) event
{
    NSWindow* nswindow = static_cast<NSWindow*>(ptkWindow->nswindow());
    const NSPoint pos = [event locationInWindow];
    const NSRect content = [nswindow.contentView frame];
    pTK::MotionEvent evt{{static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    ptkWindow->HandlePlatformEvent<pTK::MotionEvent>(evt);
}

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace pTK::Platform
{
    WindowHandle_mac::WindowHandle_mac(WindowBase* base, const std::string& name, const Size& size,
                                       const WindowInfo& flags)
        : WindowHandle(base)
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

            setTitle(name);
            Platform::ApplicationHandle_mac::SetMenuBar(flags.menus);
        }
        PTK_INFO("Initialized WindowHandle_mac");
    }

    bool WindowHandle_mac::setPosHint(const Point& pos)
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            const NSRect content = [nswindow.contentView frame];
            const CGFloat y = CGDisplayBounds(CGMainDisplayID()).size.height - (pos.y + content.size.height - 1) - 1;
            const NSRect rect = NSMakeRect(pos.x, y, 0, 0);
            const NSRect frame = [nswindow frameRectForContentRect:rect];
            [nswindow setFrameOrigin:frame.origin];
        }
        return true;
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

    bool WindowHandle_mac::setIcon(int32_t, int32_t, uint8_t*)
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

    bool WindowHandle_mac::resize(const Size& size)
    {
        @autoreleasepool
        {

            if (size != getSize())
            {
                PTK_INFO("bool WindowHandle_mac::resize(const Size& size)");
                NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
                NSRect content = [nswindow contentRectForFrameRect:[nswindow frame]];
                content.origin.y += content.size.height - size.height;
                content.size = NSMakeSize(size.width, size.height);
                [nswindow setFrame:[nswindow frameRectForContentRect:content] display:YES];
                return true;
            }

            return false;
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

    Vec2f WindowHandle_mac::getDPIScale() const
    {
        return m_scale;
    }

    Point WindowHandle_mac::getPosition() const
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            const NSRect rect = [nswindow contentRectForFrameRect:[nswindow frame]];
            using pType = pTK::Point::value_type;
            const pType y{static_cast<pType>(CGDisplayBounds(CGMainDisplayID()).size.height - (rect.origin.y + rect.size.height - 1) - 1)};
            return Point{static_cast<pType>(rect.origin.x), y};
        }
    }

    Size WindowHandle_mac::getSize() const
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            const NSRect rect = [[nswindow contentView] frame];
            return {static_cast<Size::value_type>(rect.size.width),
                    static_cast<Size::value_type>(rect.size.height)};
        }
    }

    void WindowHandle_mac::setLimits(const Size& min, const Size& max)
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

    void WindowHandle_mac::invalidate()
    {
        HandlePlatformEvent<PaintEvent>({{0, 0}, getSize()});
    }

    long WindowHandle_mac::windowID() const
    {
        return m_id;
    }

    void* WindowHandle_mac::nswindow() const
    {
        return m_NSWindow;
    }

} // namespace pTK
