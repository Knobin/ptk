//
//  core/Drawable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_DRAWABLE_HPP
#define PTK_CORE_DRAWABLE_HPP

// pTK Headers
#include "ptk/Core.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "ptk/platform/base/ContextBase.hpp"
PTK_DISABLE_WARN_END()

namespace pTK
{
    /** Drawable class implementation.

        This class is the base class for drawable widget.
        Widgets without this class cannot be drawn.
    */
    class Drawable
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

            @param canvas   SkCanvas to draw to.
        */
        virtual void onDraw(SkCanvas*) {}

        /** Function to enable drawing.

        */
        virtual void show() {}

        /** Function to disable drawing and hide it.

        */
        virtual void hide() {};

        /** Function for checking if it is visible.

            @return    status
        */
        virtual bool visible() const { return false; };
    };
}

#endif // PTK_CORE_DRAWABLE_HPP
