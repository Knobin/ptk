//
//  src/platform/mac/RasterPolicy_mac.mm
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "RasterPolicy_mac.hpp"

// macOS Headers
#import <Cocoa/Cocoa.h>

namespace pTK::Platform
{
    RasterPolicy_mac::RasterPolicy_mac(NSWindow *window)
        : m_window{window}
    {
        m_gc = nil;
    }

    RasterPolicy_mac::~RasterPolicy_mac()
    {
        @autoreleasepool {
            CGContextRelease(m_gc);

            delete [] static_cast<uint32_t*>(pixels);;
        }
    }

    bool RasterPolicy_mac::resize(const Size& newSize)
    {
        @autoreleasepool {
            delete [] static_cast<uint32_t*>(pixels);
            CGContextRelease(m_gc);

            const std::size_t width{static_cast<std::size_t>(newSize.width)};
            const std::size_t height{static_cast<std::size_t>(newSize.height)};

            uint32_t *buffer = new (std::nothrow) uint32_t[width * height];
            if (!buffer)
                return false;

            pixels = static_cast<void*>(buffer);
            size = sizeof(uint32_t) * width * height;

            CGColorSpaceRef rgb = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
            uint32_t bmInfo = kCGImageByteOrder32Big | kCGImageAlphaPremultipliedLast;
            m_gc = CGBitmapContextCreate(pixels, width, height, 8, sizeof(uint32_t) * width, rgb, bmInfo);
            CGColorSpaceRelease(rgb);

            return static_cast<bool>(m_gc);
        }
    }

    void RasterPolicy_mac::swapBuffers() const
    {
        @autoreleasepool {
            CGImageRef image = CGBitmapContextCreateImage(m_gc);
            m_window.contentView.wantsLayer = YES;
        #ifdef PTK_COMPILER_CLANG
        #pragma clang diagnostic push
        #pragma clang diagnostic ignored "-Wold-style-cast"
            m_window.contentView.layer.contents = (__bridge id)image;
        #pragma clang diagnostic pop
        #endif
            CGImageRelease(image);
        }
    }
}
