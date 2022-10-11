//
//  widgets/Rectangle.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// pTK Headers
#include "ptk/widgets/Rectangle.hpp"

namespace pTK
{
    Rectangle::Rectangle()
        : Shape(),
          m_cornerRadius{0.0f}
    {}

    void Rectangle::onDraw(SkCanvas* canvas)
    {
        // Set Size and Position
        SkPoint pos{convertToSkPoint(getPosition())};
        SkPoint size{convertToSkPoint(getSize())};
        size += pos; // skia needs the size to be pos+size.

        // Outline
        const float outlineThickness{getOutlineThickness()};
        const float halfOutlineThickness{outlineThickness / 2.0f};
        pos.fX += halfOutlineThickness;
        pos.fY += halfOutlineThickness;
        size.fX -= halfOutlineThickness;
        size.fY -= halfOutlineThickness;

        // Set Color
        SkPaint paint{};
        paint.setAntiAlias(true);
        const Color color{getColor()};
        paint.setARGB(color.a, color.r, color.g, color.b);

        // Draw Rect
        SkRect rect{};
        rect.set(pos, size);
        paint.setStrokeWidth(outlineThickness);
        if (outlineThickness > 0.0f)
            paint.setStyle(SkPaint::kFill_Style);
        else
            paint.setStyle(SkPaint::kStrokeAndFill_Style);

        canvas->drawRoundRect(rect, m_cornerRadius, m_cornerRadius, paint);

        if (outlineThickness > 0.0f)
        {
            // Draw Outline
            const Color outColor{getOutlineColor()};
            paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
            paint.setStyle(SkPaint::kStroke_Style);
            canvas->drawRoundRect(rect, m_cornerRadius, m_cornerRadius, paint);
        }
    }

    void Rectangle::setCornerRadius(float radius)
    {
        if (radius >= 0)
            m_cornerRadius = radius;
    }

    float Rectangle::getCornerRadius() const
    {
        return m_cornerRadius;
    }

    // Comparison operators.
    bool operator==(const Rectangle& lhs, const Rectangle& rhs)
    {
        return ((lhs.getColor() == rhs.getColor()) && (lhs.getOutlineColor() == rhs.getOutlineColor()) &&
                (lhs.getOutlineThickness() == rhs.getOutlineThickness()) &&
                (lhs.getCornerRadius() == rhs.getCornerRadius()));
    }

    bool operator!=(const Rectangle& lhs, const Rectangle& rhs)
    {
        return !(lhs == rhs);
    }
} // namespace pTK
