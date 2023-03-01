//
//  src/platform/mac/ApplicationHandleMac.mm
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

// Local Headers
#include "ApplicationHandleMac.hpp"
#include "../../Log.hpp"
#include "UtilityMac.hpp"
#include "WindowHandleMac.hpp"

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/menu/NamedMenuItem.hpp"

// macOS Headers
#import <Cocoa/Cocoa.h>

// C++ Headers
#include <map>

namespace pTK::Platform::AppFactoryImpl
{
    std::unique_ptr<ApplicationHandle> Make(ApplicationBase* base, std::string_view name)
    {
        return std::make_unique<ApplicationHandleMac>(base, name);
    }
} // namespace pTK::Platform::AppFactoryImpl

@interface AppDelegate : NSObject <NSApplicationDelegate, NSWindowDelegate>
{
    std::map<void*, pTK::NamedMenuItem*>* clickItemMap;
}

- (AppDelegate*)initWithData:(std::map<void*, pTK::NamedMenuItem*>*)initData;
- (IBAction)MenuItemCallback:(id)sender;

@property(nonatomic, assign) BOOL run;

@end

@implementation AppDelegate : NSObject

@synthesize run = _run;

- (AppDelegate*)initWithData:(std::map<void*, pTK::NamedMenuItem*>*)initData
{
    self = [super init];
    if (self != nil)
    {
        clickItemMap = initData;
        _run = TRUE;
    }

    return self;
}

- (IBAction)MenuItemCallback:(id)sender
{
    std::map<void*, pTK::NamedMenuItem*>::const_iterator it{clickItemMap->find(static_cast<void*>(sender))};
    if (it != clickItemMap->cend())
        it->second->notifyClick();
}

- (void)applicationDidFinishLaunching:(NSNotification*)__unused notification
{
    // @autoreleasepool {
    NSEvent* event = [NSEvent otherEventWithType:NSEventTypeApplicationDefined
                                        location:NSMakePoint(0, 0)
                                   modifierFlags:0
                                       timestamp:0
                                    windowNumber:0
                                         context:nil
                                         subtype:0
                                           data1:0
                                           data2:0];
    [NSApp postEvent:event atStart:YES];
    // }
    [NSApp stop:nil];
}

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication*)__unused sender
{
    pTK::Application* app = pTK::Application::Get();

    // Perform closing action, if already closed or closing stop the event handling.
    if (!app->isClosed() && app->close())
    {
        [NSApp stop:self];
        [NSApp abortModal];

        // Application will call terminate later.
        return NSTerminateCancel;
    }

    return NSTerminateNow;
}

- (void)applicationWillTerminate:(NSNotification*)__unused notification
{
    _run = FALSE;
}

@end

namespace pTK::Platform
{
    static struct AppData_mac
    {
        AppDelegate* delegate{nullptr};
        NSMenu* menuBar{nullptr};
        std::map<void*, NamedMenuItem*> menuItemMap{};
        std::string name{};
        bool initialized{false};
    } s_appData{};

    static void SetAppMenu(const std::string& name, NSMenu* menu)
    {
        NSString* const applicationName = [NSString stringWithstring:name];

        NSMenuItem* menuItem = [menu
            addItemWithTitle:[NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"About", nil), applicationName]
                      action:@selector(orderFrontStandardAboutPanel:)
               keyEquivalent:@""];
        [menuItem setTarget:NSApp];
        [menu addItem:[NSMenuItem separatorItem]];

        menuItem = [menu
            addItemWithTitle:[NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"Hide", nil), applicationName]
                      action:@selector(hide:)
               keyEquivalent:@"h"];
        [menuItem setTarget:NSApp];

        menuItem = [menu addItemWithTitle:NSLocalizedString(@"Hide Others", nil)
                                   action:@selector(hideOtherApplications:)
                            keyEquivalent:@"h"];
        [menuItem setKeyEquivalentModifierMask:NSEventModifierFlagCommand | NSEventModifierFlagOption];
        [menuItem setTarget:NSApp];

        menuItem = [menu addItemWithTitle:NSLocalizedString(@"Show All", nil)
                                   action:@selector(unhideAllApplications:)
                            keyEquivalent:@""];
        [menuItem setTarget:NSApp];

        [menu addItem:[NSMenuItem separatorItem]];

        menuItem = [menu
            addItemWithTitle:[NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"Quit", nil), applicationName]
                      action:@selector(terminate:)
               keyEquivalent:@"q"];
        [menuItem setTarget:NSApp];
    }

    static void SetWindowMenu(NSMenu* menu)
    {
        [[maybe_unused]] NSMenuItem* menuItem = [menu addItemWithTitle:NSLocalizedString(@"Minimize", nil)
                                                                action:@selector(performMinimize:)
                                                         keyEquivalent:@"m"];

        menuItem = [menu addItemWithTitle:NSLocalizedString(@"Zoom", nil)
                                   action:@selector(performZoom:)
                            keyEquivalent:@""];

        [menu addItem:[NSMenuItem separatorItem]];

        menuItem = [menu addItemWithTitle:NSLocalizedString(@"Bring All to Front", nil)
                                   action:@selector(arrangeInFront:)
                            keyEquivalent:@""];
    }

    static void SetHelpMenu(const std::string& name, NSMenu* menu)
    {
        NSString* const applicationName = [NSString stringWithstring:name];

        NSMenuItem* menuItem = [menu
            addItemWithTitle:[NSString stringWithFormat:@"%@ %@", NSLocalizedString(@"Help", nil), applicationName]
                      action:@selector(showHelp:)
               keyEquivalent:@"?"];
        [menuItem setTarget:NSApp];
    }

    ApplicationHandleMac::ApplicationHandleMac(ApplicationBase* base, std::string_view name)
        : ApplicationHandle(base)
    {
        @autoreleasepool
        {
            [NSApplication sharedApplication];

            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];
            [NSWindow setAllowsAutomaticWindowTabbing:NO];

            s_appData.delegate = [[AppDelegate alloc] initWithData:&s_appData.menuItemMap];
            [NSApp setDelegate:s_appData.delegate];

            s_appData.menuBar = [[NSMenu alloc] initWithTitle:@"AMainMenu"];
            s_appData.name = name;

            // Setting the menu bar here will cause it look weird as its creating the "small" menu and
            // then re-creating the menu later with (possibly) more items and it "glitches".
            // For now the Window will set the menu bar.
            // SetMenuBar(nullptr);

            if (![[NSRunningApplication currentApplication] isFinishedLaunching])
                [NSApp run];
        }

        PTK_INFO("Initialized ApplicationHandleMac");
    }

    ApplicationHandleMac::~ApplicationHandleMac()
    {
        @autoreleasepool
        {
            [NSApp setDelegate:nil];
            [s_appData.delegate release];

            [s_appData.menuBar release];
        }

        PTK_INFO("Destroyed ApplicationHandleMac");
    }

    void ApplicationHandleMac::pollEvents()
    {
        @autoreleasepool
        {
            NSEvent* event = nil;
            do
            {
                event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                           untilDate:[NSDate distantPast]
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES];
                [NSApp sendEvent:event];
            } while (event != nil);
        }
    }

    void ApplicationHandleMac::waitEvents()
    {
        @autoreleasepool
        {
            NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate distantFuture]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES];
            [NSApp sendEvent:event];

            pollEvents();
        }
    }

    void ApplicationHandleMac::waitEventsTimeout(uint32_t ms)
    {
        @autoreleasepool
        {
            NSEvent* event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate dateWithTimeIntervalSinceNow:(ms / 1000)]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES];
            [NSApp sendEvent:event];

            pollEvents();
        }
    }

    std::optional<std::pair<char, NSEventModifierFlags>> ConvertShortcut(const Shortcut& shortcut)
    {
        if (!shortcut.modifiers().empty() && IsKeyCodeAlpha(shortcut.key()))
        {
            constexpr std::pair<std::array<KeyCode, 2>, NSEventModifierFlags> shiftKeys{
                {pTK::KeyCode::LeftShift, pTK::KeyCode::RightShift},
                NSEventModifierFlagShift};
            constexpr std::pair<std::array<KeyCode, 2>, NSEventModifierFlags> controlKeys{
                {pTK::KeyCode::LeftControl, pTK::KeyCode::RightControl},
                NSEventModifierFlagControl};
            constexpr std::pair<std::array<KeyCode, 2>, NSEventModifierFlags> altKeys{
                {pTK::KeyCode::LeftAlt, pTK::KeyCode::RightAlt},
                NSEventModifierFlagOption};
            constexpr std::pair<std::array<KeyCode, 2>, NSEventModifierFlags> commandKeys{
                {pTK::KeyCode::LeftSuper, pTK::KeyCode::RightSuper},
                NSEventModifierFlagCommand};
            constexpr std::array<std::pair<std::array<KeyCode, 2>, NSEventModifierFlags>, 4> keys{
                shiftKeys, controlKeys, altKeys, commandKeys};

            NSEventModifierFlags flag{};
            for (KeyCode modifier : shortcut.modifiers())
            {
                if (IsKeyCodeModifier(modifier))
                {
                    bool found{false};
                    for (const auto& pair : keys)
                    {
                        auto it = std::find(pair.first.cbegin(), pair.first.cend(), modifier);
                        if (it != pair.first.cend())
                        {
                            flag |= pair.second;
                            found = true;
                            break;
                        }
                    }
                    if (!found)
                        return std::nullopt;
                }
            }

            return {{std::tolower(KeyCodeToAlpha(shortcut.key())), flag}};
        }

        return std::nullopt;
    }

    void SetNamedOrCheckboxItem(NSMenu* submenu, const std::shared_ptr<MenuItem>& item, bool isCheckbox)
    {
        if (NamedMenuItem* nmItem = dynamic_cast<NamedMenuItem*>(item.get()))
        {
            NSString* const itemName = [NSString stringWithstring:nmItem->name()];
            std::string key{};
            NSEventModifierFlags flag{};
            bool valid{false};

            if (auto shortcutData = ConvertShortcut(nmItem->shortcut()))
            {
                valid = true;
                flag = shortcutData->second;
                key = std::string{shortcutData->first};
            }

            NSMenuItem* mItem = [submenu addItemWithTitle:itemName
                                                   action:@selector(MenuItemCallback:)
                                            keyEquivalent:[NSString stringWithstring:key]];
            if (valid)
                [mItem setKeyEquivalentModifierMask:flag];

            if (isCheckbox)
            {
                if (item->status() == MenuItemStatus::Checked)
                    [mItem setState:NSControlStateValueOn];
                else if (item->status() == MenuItemStatus::Unchecked)
                    [mItem setState:NSControlStateValueOff];

                item->onUpdate("NS::Toggle", [mItem, nmItem]() {
                    MenuItemStatus status{nmItem->status()};
                    if (status == MenuItemStatus::Checked)
                        [mItem setState:NSControlStateValueOn];
                    else if (status == MenuItemStatus::Unchecked)
                        [mItem setState:NSControlStateValueOff];
                    [mItem setTitle:[NSString stringWithstring:nmItem->name()]];
                });
            }
            else
            {
                item->onUpdate("NS::Update", [mItem, nmItem]() {
                    [mItem setTitle:[NSString stringWithstring:nmItem->name()]];
                });
            }

            [mItem setTarget:[NSApp delegate]];
            s_appData.menuItemMap[static_cast<void*>(mItem)] = nmItem;
        }
    }

    void SetMenu(NSMenu* parent, Menu* menu)
    {
        NSString* const menuName = [NSString stringWithstring:menu->name()];

        NSMenuItem* menuItem = [parent addItemWithTitle:menuName action:nil keyEquivalent:@""];
        NSMenu* submenu = [[NSMenu alloc] initWithTitle:menuName];

        // NSMenuItem *menuItem = [menu addItemWithTitle : NSLocalizedString(@"Minimize", nil)
        //                        action : @selector(performMinimize:) keyEquivalent : @"m"];

        for (const auto& item : *menu)
        {
            switch (item->type())
            {
                case MenuItemType::Text:
                {
                    SetNamedOrCheckboxItem(submenu, item, false);
                    break;
                }
                case MenuItemType::Checkbox:
                {
                    SetNamedOrCheckboxItem(submenu, item, true);
                    break;
                }
                case MenuItemType::Separator:
                    [submenu addItem:[NSMenuItem separatorItem]];
                    break;
                case MenuItemType::Menu:
                {
                    if (Menu* sMenu = dynamic_cast<Menu*>(item.get()))
                        SetMenu(submenu, sMenu);
                    break;
                }
            }
        }

        [parent setSubmenu:submenu forItem:menuItem];
    }

    void ApplicationHandleMac::SetMenuBar(const std::shared_ptr<MenuBar>& menuBar)
    {
        [s_appData.menuBar removeAllItems];
        s_appData.menuItemMap.clear();

        NSMenuItem* menuItem = [s_appData.menuBar addItemWithTitle:@"Apple" action:nil keyEquivalent:@""];
        NSMenu* submenu = [[NSMenu alloc] initWithTitle:@"Apple"];

        // Application
        SetAppMenu(s_appData.name, submenu);
        [s_appData.menuBar setSubmenu:submenu forItem:menuItem];

        // MenuBar stuff here.
        if (menuBar)
        {
            for (const auto& menu : *menuBar)
            {
                SetMenu(s_appData.menuBar, menu.get());
            }
        }

        // Window
        menuItem = [s_appData.menuBar addItemWithTitle:@"Window" action:nil keyEquivalent:@""];
        submenu = [[NSMenu alloc] initWithTitle:NSLocalizedString(@"Window", @"The Window menu")];
        SetWindowMenu(submenu);
        [s_appData.menuBar setSubmenu:submenu forItem:menuItem];
        [NSApp setWindowsMenu:submenu];

        // Help
        menuItem = [s_appData.menuBar addItemWithTitle:@"Help" action:nil keyEquivalent:@""];
        submenu = [[NSMenu alloc] initWithTitle:NSLocalizedString(@"Help", @"The Help menu")];
        SetHelpMenu(s_appData.name, submenu);
        [s_appData.menuBar setSubmenu:submenu forItem:menuItem];

        [NSApp setMainMenu:s_appData.menuBar];
        [NSMenu setMenuBarVisible:YES];
    }

    void ApplicationHandleMac::terminate()
    {
        [NSApp terminate:nullptr];
    }

} // namespace pTK::Platform
