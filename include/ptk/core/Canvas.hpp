//
//  core/Canvas.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-03-02.
//

#ifndef PTK_CORE_CANVAS_HPP
#define PTK_CORE_CANVAS_HPP

// pTK Headers
#include "ptk/core/Text.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/util/Point.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Vec2.hpp"

// Skia Forward Declarations
class SkCanvas;
class SkFont;
class SkImage;

namespace pTK
{
    /** Canvas class.

        Wrapper that contains convenience functions for SkCanvas.
    */
    class Canvas
    {
    public:
        /** Constructs Canvas with SkCanvas.

            @param canvas   valid pointer to SkCanvas
            @return         initialized Canvas with SkCanvas
        */
        explicit Canvas(SkCanvas* canvas)
            : skCanvas{canvas}
        {}

        // SkCanvas member variable.
        SkCanvas* skCanvas{nullptr};

        /** Function for drawing a rectangle.

            @param pos      draw rectangle at
            @param size     size of the rectangle
            @param color    color of the rectangle
        */
        void drawRect(Point pos, Size size, Color color) const;

        /** Function for drawing a rectangle with outline.

            @param pos                  draw rectangle at
            @param size                 size of the rectangle
            @param color                color of the rectangle
            @param outlineColor         color of the outline
            @param outlineThickness     thickness of the outline (>0)
        */
        void drawRect(Point pos, Size size, Color color, Color outlineColor, float outlineThickness) const;

        /** Function for drawing a rectangle with outline.

            @param pos                  draw rectangle at
            @param size                 size of the rectangle
            @param color                color of the rectangle
            @param outlineColor         color of the outline
            @param outlineThickness     thickness of the outline (>0)
        */
        void drawRect(Vec2f pos, Vec2f size, Color color, Color outlineColor, float outlineThickness) const;

        /** Function for drawing a rectangle with rounded edges.

            @param pos              draw rectangle at
            @param size             size of the rectangle
            @param color            color of the rectangle
            @param cornerRadius     radius
        */
        void drawRoundRect(Point pos, Size size, Color color, float cornerRadius) const;

        /** Function for drawing a rectangle with rounded edges.

            @param pos              draw rectangle at
            @param size             size of the rectangle
            @param color            color of the rectangle
            @param cornerRadius     radius
        */
        void drawRoundRect(Vec2f pos, Vec2f size, Color color, float cornerRadius) const;

        /** Function for drawing a rectangle with rounded edges and outline.

            @param pos                  draw rectangle at
            @param size                 size of the rectangle
            @param color                color of the rectangle
            @param cornerRadius         radius
            @param outlineColor         color of the outline
            @param outlineThickness     thickness of the outline (>0)
        */
        void drawRoundRect(Point pos, Size size, Color color, float cornerRadius, Color outlineColor,
                           float outlineThickness) const;

        /** Function for drawing a rectangle with rounded edges and outline.

            @param pos                  draw rectangle at
            @param size                 size of the rectangle
            @param color                color of the rectangle
            @param cornerRadius         radius
            @param outlineColor         color of the outline
            @param outlineThickness     thickness of the outline (>0)
        */
        void drawRoundRect(Vec2f pos, Vec2f size, Color color, float cornerRadius, Color outlineColor,
                           float outlineThickness) const;

        /** Function for drawing a line of text.

            @param data     str to draw, size of the ptr and encoding
            @param color    color of the text
            @param pos      draw text at
            @param font     valid pointer to SkFont
            @return         advance
        */
        float drawTextLine(const Text::StrData& data, const Color& color, const Vec2f& pos, const SkFont* font) const;

        /** Function for drawing a line of text.

            @param data         str to draw, size of the ptr and encoding
            @param color        color of the text
            @param pos          draw text at
            @param font         valid pointer to SkFont
            @param outlineSize  outline size
            @param outColor     outline color
            @return             advance
        */
        float drawTextLine(const Text::StrData& data, const Color& color, const Vec2f& pos, const SkFont* font,
                           float outlineSize, const Color& outColor) const;

        /** Function for drawing a line of text.

            @param data         str to draw, size of the ptr and encoding
            @param pos          draw text at
            @param font         valid pointer to SkFont
            @param paint        valid pointer to SkPaint
            @return             advance
        */
        float drawTextLineWithPaint(const Text::StrData& data, const Vec2f& pos, const SkFont* font,
                                    SkPaint* paint) const;

        /** Function for drawing a SkImage.

            @param pos      draw rectangle at
            @param size     size of the rectangle
            @param image    valid pointer to SkImage
        */
        void drawImage(Point pos, Size size, const SkImage* image) const;

        /** Function for drawing a SkImage.

            @param pos      draw rectangle at
            @param size     size of the rectangle
            @param image    valid pointer to SkImage
        */
        void drawImage(Vec2f pos, Vec2f size, const SkImage* image) const;

        /** Function for saving the current matrix and clip on the stack.

        */
        void save() const;

        /** Function for saving the current matrix and clip on the stack.

            Applies graphics state (paint) to the new layer.

            @param paint        valid pointer to SkPaint
        */
        void saveLayer(SkPaint* paint) const;

        /** Function for removing changes to matrix and clip from the stack.

        */
        void restore() const;

        /** Function for adding a translation transformation to the current matrix.

            @param x    units to move horizontally
            @param y    units to move vertically
        */
        void translate(float x, float y) const;

        /** Function for adding a scaling transformation to the current matrix.

            @param x    scaling factor in the horizontal direction
            @param y    scaling factor in the vertical direction
        */
        void scale(float x, float y) const;

        /** Function for adding a rotation transformation to the current matrix.

            Positive degrees rotates clockwise.

            @param degrees    degrees to rotate
        */
        void rotate(float degrees) const;

        /** Function for adding a rotation transformation to the current matrix.

            Positive degrees rotates clockwise.

            @param degrees      degrees to rotate
            @param x            point on horizontal axis to rotate about.
            @param y            point on vertical axis to rotate about.
        */
        void rotate(float degrees, float x, float y) const;

        /** Function for adding a rotation transformation to the current matrix.

            Transformation matrix described by:

            | a | c | e |
            | b | d | f |
            | 0 | 0 | 1 |

            @param a    horizontal scaling (m11)
            @param b    vertical skewing (m12)
            @param c    horizontal skewing (m21)
            @param d    vertical scaling (m22)
            @param e    horizontal translation (dx)
            @param f    vertical translation (dy)
        */
        void transform(float a, float b, float c, float d, float e, float f) const;
    };
} // namespace pTK

#endif // PTK_CORE_CANVAS_HPP
