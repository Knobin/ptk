//
//  widgets/Rectangle.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// Local Headers
#include "ptk/widgets/Rectangle.hpp"

namespace pTK
{
    Rectangle::Rectangle()
        : Shape(), m_cornerRadius{0}
    {
    }
    
    void Rectangle::onDraw(SkCanvas* canvas)
    {
        SkPaint paint;
        paint.setAntiAlias(true);
        Color color = getColor();
        paint.setARGB(color.a, color.r, color.g, color.b);
        
        Vec2<float> tmp_pos = getPosition();
        SkPoint pos = {tmp_pos.x, tmp_pos.y};
        Vec2<float> tmp_size = getSize();
        SkPoint size = pos + SkPoint{tmp_size.x, tmp_size.y};
        
        SkRect rect;
        rect.set(pos, size);
        canvas->drawRoundRect(rect, m_cornerRadius, m_cornerRadius, paint);
        
        if (getOutlineThickness() > 0)
        {
            paint.setStrokeWidth((float)getOutlineThickness());
            Color o_color = getOutlineColor();
            paint.setARGB(o_color.a, o_color.r, o_color.g, o_color.b);
            paint.setStyle(SkPaint::kStroke_Style);
            
            canvas->drawRoundRect(rect, m_cornerRadius, m_cornerRadius, paint);
        }
    }
    
    void Rectangle::setCornerRadius(float radius)
    {
        if (radius > 0)
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
