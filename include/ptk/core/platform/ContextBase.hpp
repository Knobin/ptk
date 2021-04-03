//
//  core/platform/ContextBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

#ifndef PTK_CORE_PLATFORM_CONTEXTBASE_HPP
#define PTK_CORE_PLATFORM_CONTEXTBASE_HPP

// pTK Headers
#include "ptk/Core.hpp"
#include "ptk/util/SingleObject.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/util/Point.hpp"

// C++ Headers
#include <memory>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    /** ContextBase class implementation.

        This class handles the drawing.
        Kind of an API class that implements some common functions.
    */
    class ContextBase : public SingleObject
    {
    public:
        /** Constructs ContextBase with default values.

            @return    default initialized ContextBase
        */
        explicit ContextBase(const Size& size);

        /** Destructor for ContextBase.

        */
        virtual ~ContextBase() = default;

        /** Function for resizing the Context.

            @param width   New width of context
            @param width   New height of context
        */
        virtual void resize(const Size& size) = 0;

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        virtual sk_sp<SkSurface> surface() const = 0;

        /** Function for retrieving the size of the context.

            @return    size of canvas
        */
        const Size& getSize() const;

        /** Function for swapping the buffers.

        */
        virtual void swapBuffers() {};

    protected:
        /** Function for setting the size of the context.

            Note: this function is only for internal use to update the size so that the getSize works.

            @param size     size to set.
        */
        void setSize(const Size& size);

    private:
        Size m_size;
    };

    // Functions for converting utility classes to SkPoint for drawing.
    SkPoint convertToSkPoint(const Point& pos, const Vec2f& scale = {1.0f, 1.0f});
    SkPoint convertToSkPoint(const Size& size, const Vec2f& scale = {1.0f, 1.0f});
}

#endif // PTK_CORE_PLATFORM_CONTEXTBASE_HPP
