//
//  core/canvas.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

#ifndef PTK_CORE_CANVAS_HPP
#define PTK_CORE_CANVAS_HPP

// Local Headers
#include "ptk/util/vec2.hpp"

// C++ Headers
#include <memory>

// OpenGL Headers
#include <OpenGL/gl.h>

// Skia
#include "include/gpu/GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"

namespace pTK
{
    /** Canvas class implementation.
     
     This class is to handle the SkCanvas for easy access with
     functions for common operations.
     */
    class Canvas
    {
    public:
        /** Constructs Canvas with default values.
         
         @return    default initialized Canvas
         */
        Canvas(const Vec2<uint32_t>& size);
        ~Canvas();
        
        /** Function for resizing the Canvas.
         
         @param width   New width of canvas
         @param width   New height of canvas
         */
        void resize(const Vec2<uint32_t>& size);
        
        /** Function for clearing the Canvas.
         
         */
        void clear();
        
        /** Function for retrieving the SkCanvas of the Canvas.
         
         @return    SkCanvas property
         */
        SkCanvas* sk_canvas() const;
    private:
        // Skia
        GrContext* m_context;
        SkSurface* m_surface;
        SkCanvas* m_canvas;
        GrGLFramebufferInfo m_info;
        SkColorType m_color_type;
    };
}

#endif // PTK_CORE_CANVAS_HPP
