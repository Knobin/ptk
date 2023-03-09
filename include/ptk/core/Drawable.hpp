//
//  core/Drawable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_DRAWABLE_HPP
#define PTK_CORE_DRAWABLE_HPP

// pTK Headers
#include "ptk/core/Canvas.hpp"
#include "ptk/core/Defines.hpp"

namespace pTK
{
    /** Drawable class implementation.

        This class is the base class for drawable widget.
        Widgets without this class cannot be drawn.
    */
    class PTK_API Drawable
    {
    public:
        /** Constructs Drawable with default values.

            @return    default initialized Drawable
        */
        Drawable() = default;

        /** Destructor for Drawable.

        */
        virtual ~Drawable() = default;

        /** Draw function.

            Function is called when it is time to draw.

            @param canvas   valid Canvas pointer to draw to
        */
        virtual void onDraw(Canvas* UNUSED(canvas)) {}

        /** Function to enable drawing.

        */
        virtual void show() {}

        /** Function to disable drawing and hide it.

        */
        virtual void hide() {}

        /** Function for checking if it is visible.

            @return    status
        */
        [[nodiscard]] virtual bool visible() const { return false; };
    };
} // namespace pTK

#endif // PTK_CORE_DRAWABLE_HPP
