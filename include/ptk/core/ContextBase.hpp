//
//  core/ContextBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-02-11.
//

#ifndef PTK_CORE_CONTEXTBASE_HPP
#define PTK_CORE_CONTEXTBASE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/util/Point.hpp"
#include "ptk/util/SingleObject.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Vec2.hpp"

// C++ Headers
#include <memory>

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkColorSpace.h"
#include "include/core/SkSurface.h"
PTK_DISABLE_WARN_END()

namespace pTK
{
    /** ContextBackendType enum class implementation.

        Available backend types for ContextBase.
        Additional types in the future must be added here.
    */
    enum class ContextBackendType : uint8_t
    {
        Raster = 0,
        GL,
        Metal
    };

    /** ContextBase class implementation.

        This class handles the drawing.
        Kind of an API class that implements some common functions.
    */
    class PTK_API ContextBase : public SingleObject
    {
    public:
        /** Constructs ContextBase with default values.

            @param type     backend type of the context
            @param size     size of the context
            @return         default initialized ContextBase
        */
        ContextBase(ContextBackendType type, const Size& size);

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
        virtual void swapBuffers() {}

        /** Function for retrieving the backend type of the context.

            @return    backend type of the context
        */
        [[nodiscard]] ContextBackendType type() const noexcept { return m_type; }

    protected:
        /** Function for setting the size of the context.

            Note: this function is only for internal use to update the size so that the getSize works.

            @param size     size to set.
        */
        void setSize(const Size& size);

    private:
        Size m_size;
        ContextBackendType m_type{};
    };

    // Functions for converting utility classes to SkPoint for drawing.
    PTK_API SkPoint convertToSkPoint(const Point& pos, const Vec2f& scale = {1.0f, 1.0f});
    PTK_API SkPoint convertToSkPoint(const Size& size, const Vec2f& scale = {1.0f, 1.0f});
    PTK_API SkPaint GetSkPaintFromColor(const Color& color);
} // namespace pTK

#endif // PTK_CORE_PLATFORM_CONTEXTBASE_HPP
