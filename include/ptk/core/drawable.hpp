//
//  core/drawable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_DRAWABLE_HPP
#define PTK_CORE_DRAWABLE_HPP

// Skia Headers
#include "include/core/SkCanvas.h"

namespace pTK
{
    /** Drawable class implementation.
     
     This class is the base class for drawable widget.
     Widgets without this class cannot be drawn.
     */
    class Drawable
    {
    public:
        virtual ~Drawable() = default;
        
        /** Draw function.
         
         Function is called when it is time to draw.
         
         @param canvas   SkCanvas to draw to.
         */
        virtual void onDraw(SkCanvas*) {}
    };
}

#endif // PTK_CORE_DRAWABLE_HPP
