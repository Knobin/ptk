//
//  widgets/Checkbox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-09-11.
//

// Local Headers
#include "ptk/widgets/Checkbox.hpp"

// TODO: Fix m_click or just delete it.

namespace pTK
{
    Checkbox::Checkbox()
        : Rectangle(), m_checkColor{0x007BFFFF}
    {
        Shape::setColor(Color(0x00000000));
    }
    
    void Checkbox::onDraw(SkCanvas* canvas)
    {
        drawStates(canvas);
    }
    
    void Checkbox::drawStates(SkCanvas* canvas)
    {
        Widget* parent = getParent();
        setParent(nullptr);
        
        if (m_state == 0)
        {
            Color temp = getColor();
            
            setColor(Color(0, 0, 0, 0));
            Rectangle::onDraw(canvas);
            
            setColor(temp);
        }else
        {
            Color outlineTemp = getOutlineColor();
            
            if (!status()) // State 1
            {
                setOutlineColor(getColor());
                Rectangle::onDraw(canvas);
            }else // State 2 and 3
            {
                Color temp = getColor();
                Color color = (m_state == 3) ? m_checkColor : temp; // Depends on this.
                
                setOutlineColor(color);
                setColor(color);
                
                drawChecked(canvas);
                setColor(temp);
            }
            
            setOutlineColor(outlineTemp);
        }
        
        setParent(parent);
    }
    
    void Checkbox::drawChecked(SkCanvas* canvas)
    {
        Size size = getSize();
        Point pos = getPosition();
        
        float posy = static_cast<float>(pos.y) + static_cast<float>(size.height)/2.0f + static_cast<float>(size.height)*0.25f;
        float posx = static_cast<float>(pos.x) + static_cast<float>(size.width)/2.0f - static_cast<float>(size.height)*0.20f;
        float longy = static_cast<float>(size.height)*0.7f*0.9f;
        float shortx = static_cast<float>(size.height)*0.3f*0.9f;
        float cornerRadius = getCornerRadius();
        float outline = getOutlineThickness()*2.0f;
        
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
            if (status)
                m_state = 3;
            else
                m_state = 0;
            
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
        int temp = m_state;
        if (m_state == 3)
            m_state = 2;
        else
            m_state = 1;
        
        m_hover = true;
        if (temp != m_state)
            draw();
        
        return true;
    }
    
    bool Checkbox::onLeaveEvent()
    {
        int temp = m_state;
        if ((m_state == 2) && (status()))
            m_state = 3;
        else if ((m_state == 2) && (!status()))
            m_state = 0;
        else if (m_state == 1)
            m_state = 0;
        
        m_hover = false;
        if (temp != m_state)
            draw();
        
        return true;
    }
    
    bool Checkbox::onClickEvent(MouseButton, const Point&)
    {
        int temp = m_state;
        m_click = true;
        if (status())
            m_state = 1;
        else
            m_state = 2;
        
        internalToggle();
        
        if (temp != m_state)
            draw();
        
        return true;
    }
    
    bool Checkbox::onReleaseEvent(MouseButton, const Point&)
    {
        int temp = m_state;
        
        if (!status())
            if (m_hover)
                m_state = 1;
        
        m_click = false;
        
        if (temp != m_state)
            draw();
        
        return true;
    }
    
    void Checkbox::internalToggle()
    {
        if (status())
            m_checked = false;
        else
            m_checked = true;
    }
}
