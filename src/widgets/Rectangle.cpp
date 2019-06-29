//
//  widgets/Rectangle.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// Local Headers
#include "ptk/widgets/Rectangle.hpp"
#include "ptk/Log.hpp"

namespace pTK
{
    Rectangle::Rectangle()
        : Shape(), m_cornerRadius{0.0f}
    {
    }
    
    void Rectangle::onDraw(const Canvas& canvas)
    {
        SkCanvas* skCanvas = canvas.skCanvas();
        Vec2f dpiScale{canvas.getDPIScale()};
        
        // Set Size and Position
        SkPoint pos{convertToSkPoint(getPosition(), dpiScale)};
        SkPoint size{convertToSkPoint(getSize(), dpiScale)};
        size += pos; // skia needs the size to be pos+size.
        
        // Outline
        float outlineThickness = getOutlineThickness();
        float halfOutlineThickness = outlineThickness/2;
        pos.fX += halfOutlineThickness*dpiScale.x;
        pos.fY += halfOutlineThickness*dpiScale.y;
        size.fX -= halfOutlineThickness*dpiScale.x;
        size.fY -= halfOutlineThickness*dpiScale.y;
        
        // Set Color
        SkPaint paint;
        paint.setAntiAlias(true);
        Color color = getColor();
        paint.setARGB(color.a, color.r, color.g, color.b);
        
        // Draw Rect
        SkRect rect;
        rect.set(pos, size);
        paint.setStrokeWidth(outlineThickness*dpiScale.x);
        if (outlineThickness > 0.0f)
            paint.setStyle(SkPaint::kFill_Style);
        else
            paint.setStyle(SkPaint::kStrokeAndFill_Style);
        
        skCanvas->drawRoundRect(rect, m_cornerRadius*dpiScale.x, m_cornerRadius*dpiScale.y, paint);
        
        if (outlineThickness > 0.0f)
        {
            // Draw Outline
            Color outColor = getOutlineColor();
            paint.setARGB(outColor.a, outColor.r, outColor.g, outColor.b);
            paint.setStyle(SkPaint::kStroke_Style);
            skCanvas->drawRoundRect(rect, m_cornerRadius*dpiScale.x, m_cornerRadius*dpiScale.y, paint);
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
        return ((lhs.getColor() == rhs.getColor()) &&
                (lhs.getOutlineColor() == rhs.getOutlineColor()) &&
                (lhs.getOutlineThickness() == rhs.getOutlineThickness()) &&
                (lhs.getCornerRadius() == rhs.getCornerRadius()));
    }
    
    bool operator!=(const Rectangle& lhs, const Rectangle& rhs)
    {
        return !(lhs == rhs);
    }
}
