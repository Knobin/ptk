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

// Forward declarations
class SkCanvas;
class SkFont;
class SkImage;

namespace pTK
{
    class Canvas
    {
    public:
        Canvas() = delete;
        Canvas(SkCanvas* canvas)
            : skCanvas{canvas}
        {}
        ~Canvas() = default;

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

        /** Function for drawing a rectangle with rounded edges.

            @param pos              draw rectangle at
            @param size             size of the rectangle
            @param color            color of the rectangle
            @param cornerRadius     radius
        */
        void drawRoundRect(Point pos, Size size, Color color, float cornerRadius) const;

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
    };
} // namespace pTK

#endif // PTK_CORE_CANVAS_HPP
