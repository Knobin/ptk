//
//  platform/mac/Platform_mac.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-08-21.
//

// Local Headers
#include "Application_mac.hpp"
#include "MainWindow_mac.hpp"

// pTK Headers
#include "ptk/Application.hpp"
#include "ptk/Window.hpp"
#include "ptk/core/Exception.hpp"
#include "ptk/Log.hpp"

// macOS Headers
#import <Cocoa/Cocoa.h>

// C++ Headers
#include <map>

@interface AppDelegate : NSObject<NSApplicationDelegate, NSWindowDelegate>
{
    pTK::Application_mac *app;
}

- (AppDelegate*)initWithData:(pTK::Application_mac*)initData;

@property (nonatomic, assign) BOOL run;

@end

@implementation AppDelegate : NSObject

@synthesize run = _run;

- (AppDelegate*)initWithData:(pTK::Application_mac*)initData {
    self = [super init];
    if (self != nil)
    {
        app = initData;
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
    pTK::Application::Get()->close();
    _run = FALSE;
    return NSTerminateCancel;
}

@end


namespace pTK
{
    AppDelegate* appDelegate;
    NSMenu* menuBar;
    
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
            
            item=[[NSMenuItem alloc] initWithTitle:@"Test" action:@selector(terminate:) keyEquivalent:@""];
            [subMenu addItem:item];
            [item release];
            
            item=[[NSMenuItem alloc] initWithTitle:@"Quit" action:@selector(terminate:) keyEquivalent:@"q"];
            [subMenu addItem:item];
            [item release];
            [subMenu release];

            appDelegate = [[AppDelegate alloc] initWithData:this];
            [NSApp setDelegate:appDelegate];

            if (![[NSRunningApplication currentApplication] isFinishedLaunching])
                [NSApp run];
        }

        PTK_INFO("Initialized Application_mac");
    }

    Application_mac::~Application_mac()
    {
        @autoreleasepool {
            [NSApp setDelegate:nil];
            [appDelegate release];

            [menuBar release];
        }

        PTK_INFO("Destroyed Application_mac");
    }

    void Application_mac::pollEvents()
    {
        @autoreleasepool {
            NSEvent *event = nil;
            do {
                event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                           untilDate:[NSDate distantPast]
                                              inMode:NSDefaultRunLoopMode
                                             dequeue:YES];
                [NSApp sendEvent:event];
            } while(event != nil);
        }
    }

    void Application_mac::waitEvents()
    {
        @autoreleasepool {
            NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate distantFuture]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES];
            [NSApp sendEvent:event];

            pollEvents();
        }
    }

    void Application_mac::waitEventsTimeout(uint ms)
    {
        @autoreleasepool {
            NSEvent *event = [NSApp nextEventMatchingMask:NSEventMaskAny
                                                untilDate:[NSDate dateWithTimeIntervalSinceNow:(ms / 1000)]
                                                   inMode:NSDefaultRunLoopMode
                                                  dequeue:YES];
            [NSApp sendEvent:event];

            pollEvents();
        }
    }

} // namespace pTK
