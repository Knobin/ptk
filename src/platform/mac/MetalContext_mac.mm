//
//  platform/mac/MetalContext_mac.mm
//  pTK
//
//  Created by Robin Gustafsson on 2020-09-12.
//

// Local Headers
#include "ptk/platform/mac/MetalContext_mac.hpp"

// pTK Headers
#include "ptk/core/Exception.hpp"

// macOS Headers
#import <Cocoa/Cocoa.h>
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>
#import <QuartzCore/CAConstraintLayoutManager.h>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/gpu/GrBackendSurface.h"
#include "include/gpu/mtl/GrMtlTypes.h"
#include "include/gpu/GrContextOptions.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    MetalContext_mac::MetalContext_mac(void* mainView, const Size& size, const Vec2f& scale)
        : ContextBase(size),
            m_props{SkSurfaceProps::kUseDeviceIndependentFonts_Flag, SkSurfaceProps::kLegacyFontHost_InitType},
            m_drawableHandle{nullptr}
    {
        init(mainView, size, scale);
    }

    MetalContext_mac::~MetalContext_mac()
    {
        @autoreleasepool {
            if (m_context)
            {
                m_context->abandonContext();
                m_context.reset();
            }

            m_mainView.layer = nil;
            m_mainView.wantsLayer = NO;

            m_metalLayer = nil;
            [m_queue release];
            [m_device release];

            PTK_INFO("Destroyed MetalContext_mac");
        } // autoreleasepool
    }

    void MetalContext_mac::init(void* mainView, const Size& size, const Vec2f& scale)
    {
        @autoreleasepool {
            m_mainView = static_cast<NSView*>(mainView);
            CGDirectDisplayID viewDisplayID = static_cast<CGDirectDisplayID>( [m_mainView.window.screen.deviceDescription[@"NSScreenNumber"] unsignedIntegerValue]);
            m_device = CGDirectDisplayCopyCurrentMetalDevice(viewDisplayID);
            // m_device = MTLCreateSystemDefaultDevice();
            m_queue = [m_device newCommandQueue];
            if (!m_queue)
                throw ContextError("Could not create command queue");
            [m_queue setLabel:@"PTK Main Queue"];

            m_metalLayer = [CAMetalLayer layer];
            m_metalLayer.device = m_device;
            m_metalLayer.pixelFormat = MTLPixelFormatBGRA8Unorm;

            NSRect rect;
            NSRect frameRect = [m_mainView frame];
            rect.size.width = size.width;
            rect.size.height = size.height;
            m_metalLayer.drawableSize = rect.size;
            m_metalLayer.frame = frameRect;

            BOOL useVsync = YES;
            m_metalLayer.displaySyncEnabled = useVsync;
            m_metalLayer.layoutManager = [CAConstraintLayoutManager layoutManager];
            m_metalLayer.autoresizingMask = kCALayerHeightSizable | kCALayerWidthSizable;
            m_metalLayer.contentsGravity = kCAGravityTopLeft;
            m_metalLayer.magnificationFilter = kCAFilterNearest;
            NSColorSpace* cs = m_mainView.window.colorSpace;
            m_metalLayer.colorspace = cs.CGColorSpace;

            m_metalLayer.contentsScale = static_cast<double>(scale.x);
            m_mainView.wantsLayer = YES;
            m_mainView.layer = m_metalLayer;

            m_context = GrContext::MakeMetal([m_device retain], [m_queue retain]);
            if (!m_context)
                throw ContextError("Could not create GrContext");

            Size fSize{static_cast<Size::value_type>(rect.size.width),
                static_cast<Size::value_type>(rect.size.height)};

            PTK_INFO("Initialized MetalContext_mac");
        } // autoreleasepool
    }

    void MetalContext_mac::resize(const Size& size)
    {
        @autoreleasepool {
            NSRect rect;
            rect.size.width = size.width;
            rect.size.height = size.height;
            m_metalLayer.drawableSize = rect.size;
            m_metalLayer.frame = m_mainView.frame;
            setSize(size);
            PTK_INFO("Sized MetalContext_mac to {}x{}", size.width, size.height);
        } // autoreleasepool
    }

    sk_sp<SkSurface> MetalContext_mac::surface() const
    {
        @autoreleasepool {
            if (m_drawableHandle != nullptr)
            {
                CFRelease(m_drawableHandle);
                m_drawableHandle = nullptr;
            }

            sk_sp<SkSurface> surface;
            if (m_context)
                surface = SkSurface::MakeFromCAMetalLayer(m_context.get(), m_metalLayer, kTopLeft_GrSurfaceOrigin, 1, kBGRA_8888_SkColorType, nullptr, nullptr, &m_drawableHandle);

            return surface;
        } // autoreleasepool
    }

    void MetalContext_mac::swapBuffers()
    {
        @autoreleasepool {
            if (m_drawableHandle == nullptr) {
              PTK_WARN("Could not acquire next Metal drawable from the SkSurface.");
              return;
            }

            id<MTLCommandBuffer> commandBuffer = [m_queue commandBuffer];
            id<CAMetalDrawable> drawable = reinterpret_cast<id<CAMetalDrawable>>(m_drawableHandle);
            CFRelease(m_drawableHandle);
            m_drawableHandle = nullptr;

            [commandBuffer commit];
            [commandBuffer waitUntilScheduled];
            [drawable present];
        } // autoreleasepool
    }
}
