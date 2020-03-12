//
//  core/ContextBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

#ifndef PTK_CORE_CONTEXTBASE_HPP
#define PTK_CORE_CONTEXTBASE_HPP

// Local Headers
#include "ptk/Core.hpp"
#include "ptk/util/Singleton.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/util/Point.hpp"

// C++ Headers
#include <memory>

// Skia Headers
#ifdef PTK_COMPILER_GCC
#pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#elif PTK_COMPILER_CLANG
#pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#elif PTK_COMPILER_MSVC
    #pragma warning( push, 0 )
#endif
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#ifdef PTK_COMPILER_GCC
    #pragma GCC diagnostic pop
#elif PTK_COMPILER_CLANG
    #pragma clang diagnostic pop
#elif PTK_COMPILER_MSVC
    #pragma warning( pop )
#endif

namespace pTK
{
    class ContextBase : public Singleton
    {
    public:
        /** Constructs ContextBase with default values.

            @return    default initialized ContextBase
        */
        ContextBase(const Size& size);

        /** Deconstructor for ContextBase.

        */
        virtual ~ContextBase() = default;

        /** Function for resizing the Context.

            @param width   New width of context
            @param width   New height of context
        */
        virtual void resize(const Size& size) = 0;

        /** Function for clearing the context.

            @param color   Clear color
        */
        void clear(const Color& color);

        /** Function for retrieving the SkCanvas of the context.

            @return    SkCanvas property
        */
        virtual SkCanvas* skCanvas() const = 0;

        /** Function for retrieving the SkSurface of the context.

            @return    SkSurface property
        */
        virtual SkSurface* skSurface() const = 0;

        /** Function for retrieving the size of the context.

            @return    size of canvas
        */
        const Size& getSize() const;

    protected:
        void setSize(const Size& size);

    private:
        Size m_size;
    };

    // Functions for converting utility classes to SkPoint for drawing.
    SkPoint convertToSkPoint(const Point& pos, const Vec2f& scale = {1.0f, 1.0f});
    SkPoint convertToSkPoint(const Size& size, const Vec2f& scale = {1.0f, 1.0f});
}

#endif // PTK_CORE_CONTEXTBASE_HPP