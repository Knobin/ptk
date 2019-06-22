//
//  widgets/rectangle.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// Local Headers
#include "ptk/widgets/rectangle.hpp"

namespace pTK
{
    Rectangle::Rectangle()
        : Shape(), m_corner_radius{0}
    {
    }
    
    void Rectangle::on_draw(SkCanvas* canvas)
    {
        SkPaint paint;
        paint.setAntiAlias(true);
        Color color = get_color();
        paint.setARGB(color.a, color.r, color.g, color.b);
        
        Vec2<float> tmp_pos = get_position();
        SkPoint pos = {tmp_pos.x, tmp_pos.y};
        Vec2<float> tmp_size = get_size();
        SkPoint size = pos + SkPoint{tmp_size.x, tmp_size.y};
        
        SkRect rect;
        rect.set(pos, size);
        canvas->drawRoundRect(rect, m_corner_radius, m_corner_radius, paint);
        
        if (get_outline_thickness() > 0)
        {
            paint.setStrokeWidth((float)get_outline_thickness());
            Color o_color = get_outline_color();
            paint.setARGB(o_color.a, o_color.r, o_color.g, o_color.b);
            paint.setStyle(SkPaint::kStroke_Style);
            
            canvas->drawRoundRect(rect, m_corner_radius, m_corner_radius, paint);
        }
    }
    
    void Rectangle::set_corner_radius(float radius)
    {
        if (radius > 0)
            m_corner_radius = radius;
    }
    
    float Rectangle::get_corner_radius() const
    {
        return m_corner_radius;
    }
    
    // Comparison operators.
    bool operator==(const Rectangle& lhs, const Rectangle& rhs)
    {
        return ((lhs.get_color() == rhs.get_color()) &&
                (lhs.get_outline_color() == rhs.get_outline_color()) &&
                (lhs.get_outline_thickness() == rhs.get_outline_thickness()) &&
                (lhs.get_corner_radius() == rhs.get_corner_radius()));
    }
    
    bool operator!=(const Rectangle& lhs, const Rectangle& rhs)
    {
        return !(lhs == rhs);
    }
}
