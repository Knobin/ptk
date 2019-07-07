//
//  core/Drawable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_DRAWABLE_HPP
#define PTK_CORE_DRAWABLE_HPP

// Skia Headers
#include "ptk/core/Canvas.hpp"

namespace pTK
{
    /** Drawable class implementation.
     
     This class is the base class for drawable widget.
     Widgets without this class cannot be drawn.
     */
    class Drawable
    {
    public:
        Drawable()
            : m_visible{true}
        {
        }
        virtual ~Drawable() = default;
        
        /** Draw function.
         
         Function is called when it is time to draw.
         
         @param canvas   SkCanvas to draw to.
         */
        virtual void onDraw(SkCanvas*) {}
        
        void show()
        {
            m_visible = true;
        }
        
        void hide()
        {
            m_visible = true;
        }
        
        bool visible() const
        {
            return m_visible;
        }
        
    private:
        bool m_visible;
    };
}

#endif // PTK_CORE_DRAWABLE_HPP
