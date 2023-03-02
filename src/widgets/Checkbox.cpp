//
//  widgets/Checkbox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-09-11.
//

// pTK Headers
#include "ptk/widgets/Checkbox.hpp"
#include "ptk/core/ContextBase.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkPath.h"
#include "include/core/SkRRect.h"
PTK_DISABLE_WARN_END()

// TODO: Fix m_click or just delete it.

namespace pTK
{
    Checkbox::Checkbox()
        : Widget()
    {
        initCallbacks();
    }

    void Checkbox::initCallbacks()
    {
        addListener<EnterEvent>([&](const EnterEvent& evt) {
            onEnterCallback(evt);
            return false;
        });
        addListener<LeaveEvent>([&](const LeaveEvent& evt) {
            onLeaveCallback(evt);
            return false;
        });
        addListener<ClickEvent>([&](const ClickEvent& evt) {
            onClickCallback(evt);
            return false;
        });
        addListener<ReleaseEvent>([&](const ReleaseEvent& evt) {
            onReleaseCallback(evt);
            return false;
        });
    }

    void Checkbox::onDraw(Canvas& canvas)
    {
        drawStates(canvas);
    }

    void Checkbox::drawStates(Canvas& canvas)
    {
        Widget* parent = getParent();
        setParent(nullptr);

        if (m_state == 0)
        {
            Color temp = getColor();

            setColor(Color(0, 0, 0, 0));
            canvas.drawRoundRect(getPosition(), getSize(), getColor(), getCornerRadius(), getOutlineColor(),
                                 getOutlineThickness());

            setColor(temp);
        }
        else
        {
            Color outlineTemp = getOutlineColor();

            if (!status()) // State 1
            {
                setOutlineColor(getColor());
                canvas.drawRoundRect(getPosition(), getSize(), getColor(), getCornerRadius(), getOutlineColor(),
                                     getOutlineThickness());
            }
            else // State 2 and 3
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

    void Checkbox::drawChecked(Canvas& canvas)
    {
        Size size = getSize();
        Point pos = getPosition();

        float posy = static_cast<float>(pos.y) + static_cast<float>(size.height) / 2.0f +
                     static_cast<float>(size.height) * 0.25f;
        float posx =
            static_cast<float>(pos.x) + static_cast<float>(size.width) / 2.0f - static_cast<float>(size.height) * 0.20f;
        float longy = static_cast<float>(size.height) * 0.7f * 0.9f;
        float shortx = static_cast<float>(size.height) * 0.3f * 0.9f;
        float cornerRadius = getCornerRadius();
        float outline = getOutlineThickness() * 2.0f;

        // Setup clip rects.
        SkRect rect;
        rect.set({posx, posy - longy}, {posx + outline, posy});
        SkRRect rects[2];
        rects[0] = SkRRect::MakeRectXY(rect, cornerRadius, cornerRadius);
        rect.set({posx - shortx, posy - outline}, {posx + outline, posy});
        rects[1] = SkRRect::MakeRectXY(rect, cornerRadius, cornerRadius);

        // Setup a path from rects.
        SkPath path;
        path.addRRect(rects[0]);
        path.addRRect(rects[1]);
        SkMatrix matrix;
        matrix.setRotate(45, posx + outline, posy);
        path.transform(matrix);

        // Add clip to canvas and draw underlaying Rectangle.

        canvas.skCanvas->save();
        canvas.skCanvas->clipPath(path, SkClipOp::kDifference, true);
        canvas.drawRoundRect(getPosition(), getSize(), getColor(), getCornerRadius(), getOutlineColor(),
                             getOutlineThickness());
        canvas.skCanvas->restore();
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

    void Checkbox::setCornerRadius(float radius)
    {
        if (radius >= 0)
            m_cornerRadius = radius;
    }

    float Checkbox::getCornerRadius() const
    {
        return m_cornerRadius;
    }

    const Color& Checkbox::getColor() const
    {
        return m_color;
    }

    void Checkbox::setColor(const Color& color)
    {
        m_color = color;
        draw();
    }

    const Color& Checkbox::getOutlineColor() const
    {
        return m_outlineColor;
    }

    void Checkbox::setOutlineColor(const Color& outline_color)
    {
        m_outlineColor = outline_color;
        draw();
    }

    float Checkbox::getOutlineThickness() const
    {
        return m_outlineThickness;
    }

    void Checkbox::setOutlineThickness(float outlineThickness)
    {
        m_outlineThickness = outlineThickness;
        draw();
    }

    void Checkbox::onEnterCallback(const EnterEvent&)
    {
        uint8_t temp = m_state;
        if (m_state == 3)
            m_state = 2;
        else
            m_state = 1;

        m_hover = true;
        if (temp != m_state)
            draw();
    }

    void Checkbox::onLeaveCallback(const LeaveEvent&)
    {
        uint8_t temp = m_state;
        if ((m_state == 2) && (status()))
            m_state = 3;
        else if ((m_state == 2) && (!status()))
            m_state = 0;
        else if (m_state == 1)
            m_state = 0;

        m_hover = false;
        if (temp != m_state)
            draw();
    }

    void Checkbox::onClickCallback(const ClickEvent&)
    {
        uint8_t temp = m_state;
        m_click = true;
        if (status())
            m_state = 1;
        else
            m_state = 2;

        internalToggle();

        if (temp != m_state)
            draw();
    }

    void Checkbox::onReleaseCallback(const ReleaseEvent&)
    {
        uint8_t temp = m_state;

        if (!status())
            if (m_hover)
                m_state = 1;

        m_click = false;

        if (temp != m_state)
            draw();
    }

    void Checkbox::onToggleEvent(bool)
    {}

    void Checkbox::onToggle(const std::function<bool(bool status)>& callback)
    {
        m_toggleCallback = callback;
    }

    void Checkbox::internalToggle()
    {
        if (status())
            m_checked = false;
        else
            m_checked = true;

        bool status = true;
        if (m_toggleCallback != nullptr)
            status = m_toggleCallback(m_checked);

        if (status)
            onToggleEvent(m_checked);
    }
} // namespace pTK
