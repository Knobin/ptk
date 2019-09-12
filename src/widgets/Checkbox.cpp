//
//  widgets/Checkbox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-09-11.
//

// Local Headers
#include "ptk/widgets/Checkbox.hpp"

// Skia Headers
#include "include/core/SkRegion.h"

namespace pTK
{
    Checkbox::Checkbox()
        : Rectangle(), m_checked{false}, m_checkColor{0x007BFFFF}
    {
    }
    
    void Checkbox::onDraw(SkCanvas* canvas)
    {
        if (status())
            drawChecked(canvas);
        else
            Rectangle::onDraw(canvas);
    }
    
    void Checkbox::drawChecked(SkCanvas* canvas)
    {
        // Set Color
        SkPaint paint;
        paint.setAntiAlias(true);
        Color color = getOutlineColor();
        paint.setARGB(color.a, color.r, color.g, color.b);
        paint.setStyle(SkPaint::kStrokeAndFill_Style);
        
        Size size = getSize();
        Point pos = getPosition();
        
        float posy = pos.y + size.height/2 + size.height*0.25;
        float posx = pos.x + size.width/2 - size.height*0.20;
        float longy = size.height*0.7*0.9;
        float shortx = size.height*0.3*0.9;
        float cornerRadius = getCornerRadius();
        float outline = getOutlineThickness()*2;
        
        // Setup clip rects.
        SkRect rect;
        rect.set({posx, posy-longy}, {posx + outline, posy});
        SkRRect rects[2];
        rects[0] = SkRRect::MakeRectXY(rect, cornerRadius, cornerRadius);
        rect.set({posx-shortx, posy-outline}, {posx + outline, posy});
        rects[1] = SkRRect::MakeRectXY(rect, cornerRadius, cornerRadius);
        
        // Setup a path from rects.
        SkPath path;
        path.addRRect(rects[0]);
        path.addRRect(rects[1]);
        SkMatrix matrix;
        matrix.setRotate(45, posx + outline, posy);
        path.transform(matrix);
        
        // Add clip to canvas and draw underlaying Rectangle.
        canvas->save();
        canvas->clipPath(path, SkClipOp::kDifference, true);
        Rectangle::onDraw(canvas);
        canvas->restore();
    }
    
    bool Checkbox::status() const
    {
        return m_checked;
    }
    
    void Checkbox::set(bool status)
    {
        if (m_checked != status)
        {
            m_checked = status;
            draw();
        }
    }
    
    bool Checkbox::toggle()
    {
        if (status())
        {
            set(false);
            return false;
        }
        
        set(true);
        return true;
    }
    
    bool Checkbox::onEnterEvent()
    {
        // TODO
        if (status())
        {
            setOutlineColor(Color{255, 255, 255, 255});
            setColor(Color{255, 255, 255, 255});
        }else
        {
            setColor(getOutlineColor());
        }
        
        m_hover = true;
        
        return true;
    }
    
    bool Checkbox::onLeaveEvent()
    {
        // TODO
        if ((!m_click) && (!status()))
            setColor(Color{0, 0, 0, 0});
        else if ((!m_click) && (status()))
        {
            setOutlineColor(m_checkColor);
            setColor(m_checkColor);
        }
        m_hover = false;
        
        return true;
    }
    
    bool Checkbox::onClickEvent(MouseButton, const Point&)
    {
        m_click = true;
        return true;
    }
    
    bool Checkbox::onReleaseEvent(MouseButton, const Point&)
    {
        toggle();
        
        if (status())
        {
            setColor(m_checkColor);
            setOutlineColor(m_checkColor);
        }else
        {
            setOutlineColor(Color{255, 255, 255, 255});
            if (m_hover)
                setColor(Color{255, 255, 255, 255});
            else
                setColor(Color{0, 0, 0, 0});
        }
        
        m_click = false;
        
        return true;
    }
}
