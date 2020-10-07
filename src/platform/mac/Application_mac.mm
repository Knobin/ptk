//
//  platform/mac/Platform_mac.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

// Local Headers
#include "Application_mac.hpp"
#include "MainWindow_mac.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/Log.hpp"

// macOS Headers
#import <Cocoa/Cocoa.h>

// C++ Headers
#include <map>

namespace pTK
{
    struct AppData
    {
        std::map<long, MainWindow_mac*> windows;
    };
}

@interface AppDelegate : NSObject<NSApplicationDelegate, NSWindowDelegate>
{
    pTK::AppData *appData;
}

- (AppDelegate*)initWithData:(pTK::AppData*)initData;

@property (nonatomic, assign) BOOL run;

@end

@implementation AppDelegate : NSObject

@synthesize run = _run;

- (AppDelegate*)initWithData:(pTK::AppData*)initData {
    self = [super init];
    if (self != nil)
    {
        appData = initData;
        _run = TRUE;
    }

    return self;
}

- (void)applicationDidFinishLaunching:(NSNotification *) __unused notification {
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

- (NSApplicationTerminateReply)applicationShouldTerminate:(NSApplication *) __unused sender {
    PTK_INFO("APP Should Terminate");
    for (const std::pair<int32, pTK::MainWindow_mac*>& pair : appData->windows)
        pair.second->parent()->close();

    _run = FALSE;
    return NSTerminateCancel;
}

@end


namespace pTK
{
    AppDelegate* appDelegate;
    NSMenu* menuBar;
    AppData appData{};

    Application_mac::Application_mac()
        : ApplicationBase()
    {
        @autoreleasepool {
            [NSApplication sharedApplication];

            [NSApp setActivationPolicy:NSApplicationActivationPolicyRegular];

            menuBar = [[NSMenu alloc] initWithTitle:@"AMainMenu"];
            [NSApp setMainMenu:menuBar];

            NSMenuItem* item;
            NSMenu* subMenu;

            item=[[NSMenuItem alloc] initWithTitle:@"Apple" action:nil keyEquivalent:@""];
            [menuBar addItem:item];
            subMenu=[[NSMenu alloc] initWithTitle:@"Apple"];
            [menuBar setSubmenu:subMenu forItem:item];
            [item release];
            item=[[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
            [subMenu addItem:item];
            [item release];
            [subMenu release];

            appDelegate = [[AppDelegate alloc] initWithData:&appData];
            [NSApp setDelegate:appDelegate];

            if (![[NSRunningApplication currentApplication] isFinishedLaunching])
                [NSApp run];
        }

        PTK_INFO("macOS Application initialized");
    }

    Application_mac::~Application_mac()
    {
        @autoreleasepool {
            [NSApp setDelegate:nil];
            [appDelegate release];

            [menuBar release];
        }

        PTK_INFO("macOS Application destroyed");
    }

    void Application_mac::close()
    {
        // TODO: close app.
    }

    int Application_mac::messageLoop()
    {
        // Multiple windows is currently unsupported.
        // It is guaranteed to be at least one window here.
        Window *window{windows().at(1)};
        PaintEvent evt{Point{0, 0}, window->getSize()};
        window->sendEvent(&evt);
        window->show();
        
        while (!window->shouldClose())
        {
            @autoreleasepool {
                window->handleEvents();
                window->waitEvents();
            }
        }
        
        window->hide();
        
        return 0;
    }

    void Application_mac::addMacWindow(long key, MainWindow_mac* window)
    {
        PTK_INFO("Added MainWindow_mac id:{} to Application_mac", key);
        appData.windows.insert({key, window});
    }

    void Application_mac::removeMacWindow(long key)
    {
        auto it{appData.windows.find(key)};
        if (it != appData.windows.end())
        {
            PTK_INFO("Removed MainWindow_mac id:{} from Application_mac", it->first);
            appData.windows.erase(it);
        }
    }
}
