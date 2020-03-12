//
//  core/CanvasBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2020-02-09.
//

#ifndef PTK_CORE_CANVASBASE_HPP
#define PTK_CORE_CANVASBASE_HPP

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
    class CanvasBase : public Singleton
    {
    public:
        /** Constructs CanvasBase with default values.

            @return    default initialized CanvasBase
        */
        CanvasBase(const Size& size);

        /** Deconstructor for CanvasBase.

        */
        virtual ~CanvasBase() = default;

        /** Function for resizing the Canvas.

            @param width   New width of canvas
            @param width   New height of canvas
        */
        virtual void resize(const Size& size) = 0;

        /** Function for clearing the Canvas.

            @param color   Clear color
        */
        void clear(const Color& color);

        /** Function for retrieving the SkCanvas of the Canvas.

            @return    SkCanvas property
        */
        virtual SkCanvas* skCanvas() const = 0;

        /** Function for retrieving the SkSurface of the Canvas.

            @return    SkSurface property
        */
        virtual SkSurface* skSurface() const = 0;

        /** Function for retrieving the size of the Canvas.

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

#endif // PTK_CORE_CANVASBASE_HPP