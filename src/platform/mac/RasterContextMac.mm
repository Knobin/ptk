//
//  src/platform/mac/RasterContextMac.mm
//  pTK
//
//  Created by Robin Gustafsson on 2020-10-10.
//

// Local Headers
#include "RasterContextMac.hpp"

// macOS Headers
#import <Cocoa/Cocoa.h>

namespace pTK::Platform
{
    RasterContextMac::RasterContextMac(NSWindow* window, const Size& size)
        : RasterContext(kN32_SkColorType, size),
          m_window{window}
    {
        m_gc = nil;
        resize(size);
    }

    RasterContextMac::~RasterContextMac()
    {
        @autoreleasepool
        {
            CGContextRelease(m_gc);

            delete[] m_buffer;
        }
    }

    void* RasterContextMac::onResize(const Size& size)
    {
        @autoreleasepool
        {
            delete[] m_buffer;
            CGContextRelease(m_gc);

            const std::size_t width{static_cast<std::size_t>(size.width)};
            const std::size_t height{static_cast<std::size_t>(size.height)};

            m_buffer = new (std::nothrow) uint32_t[width * height];
            if (m_buffer == nullptr)
                return nullptr;

            CGColorSpaceRef rgb = CGColorSpaceCreateWithName(kCGColorSpaceSRGB);
            uint32_t bmInfo = kCGImageByteOrder32Big | kCGImageAlphaPremultipliedLast;
            m_gc = CGBitmapContextCreate(m_buffer, width, height, 8, sizeof(uint32_t) * width, rgb, bmInfo);
            CGColorSpaceRelease(rgb);

            if (!m_gc)
            {
                delete[] m_buffer;
                return nullptr;
            }

            return static_cast<void*>(m_buffer);
        }
    }

    void RasterContextMac::swapBuffers()
    {
        @autoreleasepool
        {
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
} // namespace pTK::Platform
