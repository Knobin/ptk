//
//  core/Canvas.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

#ifndef PTK_CORE_CANVAS_HPP
#define PTK_CORE_CANVAS_HPP

// Local Headers
#include "ptk/util/NonMovable.hpp"
#include "ptk/util/NonCopyable.hpp"
#include "ptk/util/Vec2.hpp"
#include "ptk/util/Point.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Color.hpp"

// C++ Headers
#include <memory>

// OpenGL Headers
#ifdef __APPLE__
    #define GL_SILENCE_DEPRECATION
    #include <OpenGL/gl.h>
#elif __linux__
    #include <GL/gl.h>
#endif

// Skia Headers
#ifdef PTK_COMPILER_GCC
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#elif PTK_COMPILER_CLANG
    #pragma clang diagnostic push
    #pragma clang diagnostic ignored "-Wgnu-anonymous-struct"
#endif
#include "include/gpu/GrContext.h"
#include "include/core/SkCanvas.h"
#include "include/core/SkSurface.h"
#ifdef PTK_COMPILER_GCC
    #pragma GCC diagnostic pop
#elif PTK_COMPILER_CLANG
    #pragma clang diagnostic pop
#endif

namespace pTK
{
    /** Canvas class implementation.
     
     This class is to handle the SkCanvas for easy access with
     functions for common operations.
     */
    class Canvas : public NonMovable, public NonCopyable
    {
    public:
        /** Constructs Canvas with default values.
         
         @return    default initialized Canvas
         */
        Canvas(const Size& size);
        ~Canvas();
        
        /** Function for resizing the Canvas.
         
         @param width   New width of canvas
         @param width   New height of canvas
         */
        void resize(const Size& size);
        
        /** Function for clearing the Canvas.
         
         @param color   Clear color
         */
        void clear(const Color& color);
        
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
        const Size& getSize() const;
        
    private:
        // Skia
        Ref<GrContext> m_context;
        sk_sp<SkSurface> m_surface;
        SkCanvas* m_canvas;
        GrGLFramebufferInfo m_info;
        SkColorType m_colorType;
        Size m_size;
    };
    
    
    // Functions for converting utility classes to SkPoint for drawing.
    SkPoint convertToSkPoint(const Point& pos, const Vec2f& scale = {1.0f, 1.0f});
    SkPoint convertToSkPoint(const Size& size, const Vec2f& scale = {1.0f, 1.0f});
}

#endif // PTK_CORE_CANVAS_HPP
