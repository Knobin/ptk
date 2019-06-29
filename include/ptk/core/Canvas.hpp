//
//  core/Canvas.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

#ifndef PTK_CORE_CANVAS_HPP
#define PTK_CORE_CANVAS_HPP

// Local Headers
#include "ptk/util/Vec2.hpp"
#include "ptk/util/Position.hpp"
#include "ptk/util/Size.hpp"

// C++ Headers
#include <memory>

// OpenGL Headers
#include <OpenGL/gl.h>

// Skia Headers
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wgnu-anonymous-struct"
#include "include/gpu/GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#pragma GCC diagnostic pop

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
        Canvas(const Vec2u& size);
        Canvas(const Canvas& canvas, const Vec2u& size);
        ~Canvas();
        
        /** Function for resizing the Canvas.
         
         @param width   New width of canvas
         @param width   New height of canvas
         */
        void resize(const Vec2u& size);
        
        /** Function for clearing the Canvas.
         
         */
        void clear();
        
        /** Function for retrieving the SkCanvas of the Canvas.
         
         @return    SkCanvas property
         */
        SkCanvas* skCanvas() const;
        
        /** Function for retrieving the SkSurface of the Canvas.
         
         @return    SkSurface property
         */
        SkSurface* skSurface() const;
        
        /** Function for retrieving the size of the Canvas.
         
         @return    size of canvas
         */
        const Vec2u& getSize() const;
        
        /** Function for setting the DPI scale.
         
         @param scale   x and y scale
         */
        void setDPIScale(const Vec2f& scale);
        
        /** Function for retrieving the DPI scale.
         
         @param scale   x and y scale
         */
        const Vec2f& getDPIScale() const;
    private:
        // Skia
        std::shared_ptr<GrContext> m_context;
        SkSurface* m_surface;
        SkCanvas* m_canvas;
        GrGLFramebufferInfo m_info;
        SkColorType m_colorType;
        Vec2u m_size;
        Vec2f m_dpiScale;
    };
    
    
    // Functions for converting utility classes to SkPoint for drawing.
    SkPoint convertToSkPoint(const Position& pos, const Vec2f& scale = {1.0f, 1.0f});
    SkPoint convertToSkPoint(const Size& size, const Vec2f& scale = {1.0f, 1.0f});
}

#endif // PTK_CORE_CANVAS_HPP
