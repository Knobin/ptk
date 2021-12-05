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
    pTK::Event evt{pTK::Event::Category::Window, pTK::Event::Type::WindowClose};
    ptkWindow->parent()->handleEvents(); // Handle all events before sending close event.
    ptkWindow->parent()->sendEvent(&evt);

    return NO;
}

- (void)windowDidResize:(NSNotification*) __unused notification
{
    NSWindow *window = static_cast<NSWindow*>(ptkWindow->nsWindow());
    const NSRect rect = [window.contentView bounds];
    const pTK::Size size{static_cast<pTK::Size::value_type>(rect.size.width),
                         static_cast<pTK::Size::value_type>(rect.size.height)};

    pTK::ResizeEvent evt{size};
    ptkWindow->parent()->sendEvent(&evt);
    ptkWindow->parent()->forceDrawAll();

}

- (void)windowDidMove:(NSNotification*) __unused notification
{
    pTK::Point pos{ptkWindow->getWinPos()};
    pTK::MoveEvent evt{pos};
    ptkWindow->parent()->sendEvent(&evt);
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
    pTK::WindowHandle_mac *ptkwindow;
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
        ptkwindow = initWindow;
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
    pTK::PaintEvent evt{pTK::Point{0,0}, ptkwindow->parent()->getSize()};
    ptkwindow->parent()->sendEvent(&evt);
}

- (void)drawRect:(NSRect) __unused rect
{
    pTK::PaintEvent evt{pTK::Point{0,0}, ptkwindow->parent()->getSize()};
    ptkwindow->parent()->sendEvent(&evt);
}


- (void)keyDown:(NSEvent *)event
{
    std::underlying_type<pTK::KeyEvent::Modifier>::type mods{GetModifiers(event.modifierFlags)};
    pTK::KeyEvent evt{pTK::Event::Type::KeyPressed, pTK::KeyMap::KeyCodeToKey(static_cast<byte>(event.keyCode)), mods};
    ptkwindow->parent()->sendEvent(&evt);

    // [self interpretKeyEvents:@[event]];
}

- (void)keyUp:(NSEvent *)event
{
    std::underlying_type<pTK::KeyEvent::Modifier>::type mods{GetModifiers(event.modifierFlags)};
    pTK::KeyEvent evt{pTK::Event::Type::KeyReleased, pTK::KeyMap::KeyCodeToKey(static_cast<byte>(event.keyCode)), mods};
    ptkwindow->parent()->sendEvent(&evt);
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
    ptkwindow->parent()->sendEvent(&evt);
}

- (BOOL)acceptsFirstMouse:(NSEvent*) __unused event
{
    return YES;
}

- (void)mouseDown:(NSEvent*) __unused event
{
    NSWindow *nswindow = static_cast<NSWindow*>(ptkwindow->nsWindow());
    const NSPoint pos = [nswindow mouseLocationOutsideOfEventStream];
    const NSRect content = [nswindow.contentView frame];
    pTK::ButtonEvent evt{pTK::Event::Type::MouseButtonPressed,
                         pTK::Mouse::Button::Left,
                         {static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    ptkwindow->parent()->sendEvent(&evt);
}

- (void)mouseUp:(NSEvent*) __unused event
{
    NSWindow *nswindow = static_cast<NSWindow*>(ptkwindow->nsWindow());
    const NSPoint pos = [nswindow mouseLocationOutsideOfEventStream];
    const NSRect content = [nswindow.contentView frame];
    pTK::ButtonEvent evt{pTK::Event::Type::MouseButtonReleased,
                         pTK::Mouse::Button::Left,
                         {static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    ptkwindow->parent()->sendEvent(&evt);
}

- (void)mouseMoved:(NSEvent*) event
{
    NSWindow *nswindow = static_cast<NSWindow*>(ptkwindow->nsWindow());
    const NSPoint pos = [event locationInWindow];
    const NSRect content = [nswindow.contentView frame];
    pTK::MotionEvent evt{{static_cast<pTK::Point::value_type>(pos.x),
                          static_cast<pTK::Point::value_type>(content.size.height - pos.y)}};
    ptkwindow->parent()->sendEvent(&evt);
}

@end

///////////////////////////////////////////////////////////////////////////////////////////////////

namespace pTK
{
    static std::unique_ptr<ContextBase> CreateMacContext(WindowInfo::Backend type, NSWindow *nswindow, const Size& size, const Vec2f& scale)
    {
#ifdef PTK_DEBUG
      if (scale.x != scale.y)
          PTK_WARN("Window scale is not the same, x: {}, y: {}", scale.x, scale.y);
#endif
        const Size scaledSize{static_cast<Size::value_type>(size.width * scale.x),
                            static_cast<Size::value_type>(size.height * scale.y)};
#ifdef PTK_METAL
        if (type == WindowInfo::Backend::Hardware)
            return std::make_unique<MetalContext_mac>(static_cast<void*>([nswindow contentView]), scaledSize, scale);
#endif

        // Software backend is always available.
        // Altough, it is slow on macOS and should be avoided.
        RasterPolicy_mac policy(nswindow);
        return std::make_unique<RasterContext<RasterPolicy_mac>>(scaledSize, policy);
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////

    WindowHandle_mac::WindowHandle_mac(Window *window, const std::string& name, const Size& size, const WindowInfo& flags)
        : WindowHandle(window)
    {
        init(name, size, flags);
    }

    WindowHandle_mac::~WindowHandle_mac()
    {
        NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
        if (nswindow != nil)
        {
            [nswindow close];
            nswindow = nil;
            m_NSWindow = nullptr;
        }
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

    bool WindowHandle_mac::setPosHint(const Point& pos)
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            const NSRect content = [nswindow.contentView frame];
            const CGFloat y = CGDisplayBounds(CGMainDisplayID()).size.height - (pos.y + content.size.height - 1) - 1;
            const NSRect rect = NSMakeRect(pos.x, y, 0, 0);
            const NSRect frame = [nswindow frameRectForContentRect:rect];
            [nswindow setFrameOrigin:frame.origin];
            return true;
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
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            NSRect content = [nswindow contentRectForFrameRect:[nswindow frame]];
            content.origin.y += content.size.height - size.height;
            content.size = NSMakeSize(size.width, size.height);
            [nswindow setFrame:[nswindow frameRectForContentRect:content] display:YES];

            const Size scaledSize{static_cast<Size::value_type>(size.width * m_scale.x),
                static_cast<Size::value_type>(size.height * m_scale.y)};
            if (scaledSize != m_context->getSize())
                m_context->resize(scaledSize);
            return true;
        }
    }

    bool WindowHandle_mac::close()
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            [nswindow close];
            nswindow = nil;
            m_NSWindow = nullptr;
            Application::Get()->removeWindow(parent());
            return true;
        }
    }

    bool WindowHandle_mac::show()
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            if (nswindow != nil)
            {
                [nswindow orderFront:nil];
                [NSApp activateIgnoringOtherApps:YES];
                [nswindow makeKeyAndOrderFront:NSApp];
                return true;
            }
            return false;
        }
    }

    bool WindowHandle_mac::hide()
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            if (nswindow != nil)
            {
                [nswindow orderOut:nil];
                return true;
            }
            return false;
        }
    }

    bool WindowHandle_mac::isHidden() const
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            return [nswindow isVisible];;
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

    bool WindowHandle_mac::setLimits(const Size& min, const Size& max)
    {
        @autoreleasepool {
            NSWindow *nswindow = static_cast<NSWindow*>(m_NSWindow);
            CGFloat frameHeight = nswindow.frame.size.height - [nswindow contentRectForFrameRect: nswindow.frame].size.height;

            const NSSize minSize = NSMakeSize(min.width, min.height + frameHeight);
            [nswindow setMinSize:minSize];

            const NSSize maxSize = NSMakeSize(max.width, max.height + ((max.height != Size::Limits::Max) ? frameHeight : 0));
            [nswindow setMaxSize:maxSize];

            return true;
        }
    }

    bool WindowHandle_mac::minimize()
    {
        @autoreleasepool {
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
            resize(parent()->getSize());
            return true;
        }
        return false;
    }

    long WindowHandle_mac::windowID() const
    {
        return m_id;
    }

    void *WindowHandle_mac::NSWindow() const
    {
        return m_NSWindow;
    }

} // namespace pTK
