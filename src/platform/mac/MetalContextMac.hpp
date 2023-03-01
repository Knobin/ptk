//
//  src/platform/mac/MetalContextMac.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-09-12.
//

#ifndef PTK_PLATFORM_MAC_METALCONTEXT_HPP
#define PTK_PLATFORM_MAC_METALCONTEXT_HPP

// pTK Headers
#include "ptk/core/ContextBase.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/gpu/GrDirectContext.h"
#include "include/ports/SkCFObject.h"
PTK_DISABLE_WARN_END()

// macOS Headers
#import <Metal/Metal.h>
#import <QuartzCore/CAMetalLayer.h>

@class NSView;

namespace pTK::Platform
{
    /** MetalContextMac class implementation.

        Context for a hardware based Metal macOS backend.
        All drawings will be done using the GPU.
    */
    class PTK_API MetalContextMac : public ContextBase
    {
    public:
        /** Constructs MetalContextMac with view and size.

            @param mainView     valid pointer to main view
            @param size         size of the context
            @param scale        scale of the context
            @return             initialized MetalContextMac with view and size
        */
        MetalContextMac(void* mainView, const Size& size, const Vec2f& scale);

        /** Win32RasterCanvas for GLCanvas.

        */
        virtual ~MetalContextMac();

        /** Function for resizing the context.

            @param size     new size
        */
        void resize(const Size& size) override;

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        sk_sp<SkSurface> surface() const override;

        /** Function for swapping the buffers

        */
        void swapBuffers() override;

    private:
        void init(void* mainView, const Size& size, const Vec2f& scale);

    private:
        // sk_sp<GrContext> m_context;
        SkSurfaceProps m_props;
        sk_sp<GrDirectContext> m_context;
        NSView* m_mainView;
        sk_cfp<id<MTLDevice>> m_device;
        sk_cfp<id<MTLCommandQueue>> m_queue;
        CAMetalLayer* m_metalLayer;
        GrContextOptions m_GrContextOptions;
        mutable GrMTLHandle m_drawableHandle = nil;
    };
} // namespace pTK::Platform

#endif // PTK_PLATFORM_MAC_METALCONTEXT_HPP
