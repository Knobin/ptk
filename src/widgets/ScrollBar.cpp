//
//  widgets/ScrollBar.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-07-01.
//

// Local Headers
#include "../include/Assert.hpp"

// pTK Headers
#include "ptk/util/Vec2.hpp"
#include "ptk/widgets/ScrollBar.hpp"

namespace pTK
{
    ScrollBar::ScrollBar(Range range)
        : Widget(),
          m_range{range}
    {}

    ScrollBar::ScrollBar(Range range, uint32_t value)
        : Widget(),
          m_range{range},
          m_value{static_cast<float>(value)}
    {}

    void ScrollBar::setRange(uint32_t start, uint32_t end)
    {
        const Range range{start, end};
        updateRange(range);
    }

    void ScrollBar::setRange(Range range)
    {
        updateRange(range);

        const auto fStart{static_cast<float>(range.start)};
        const auto fEnd{static_cast<float>(range.end)};

        if (fEnd < m_value || range.end < m_lastValue)
        {
            m_lastValue = range.end;
            m_value = fEnd;
        }
        else if (fStart > m_value || range.start > m_lastValue)
        {
            m_lastValue = range.start;
            m_value = fStart;
        }
    }

    void ScrollBar::setValue(uint32_t value)
    {
        updateValue(value);
    }

    void ScrollBar::setPageStep(uint32_t step)
    {
        m_pageStep = step;
    }

    uint64_t ScrollBar::onValueChange(const std::function<bool(const OnValueChange&)>& func)
    {
        return addListener<OnValueChange>(func);
    }

    void ScrollBar::updateRange(Range range)
    {
        m_range = range;
    }

    void ScrollBar::updateValue(uint32_t value)
    {
        const auto fValue{static_cast<float>(value)};
        setPreciseValue(fValue);
    }

    void ScrollBar::setPreciseValue(float value)
    {
        if (value != m_value)
        {
            const auto uValue{static_cast<uint32_t>(value)};
            m_value = value;
            if (m_lastValue != uValue)
            {
                int32_t offset{static_cast<int32_t>(m_lastValue) - static_cast<int32_t>(uValue)};
                m_lastValue = uValue;
                triggerEvent<OnValueChange>(OnValueChange{uValue, offset});
            }
        }
    }

    static constexpr float BarHeight(Size size, ScrollBar::Range range, uint32_t step)
    {
        const auto fTotalRange{static_cast<float>(range.end - range.start)};
        const auto fHeight{static_cast<float>(size.height)};
        const auto fStep{static_cast<float>(step)};

        if (fTotalRange > 0 && fHeight > 0 && fStep > 0)
        {
            const auto percentage{fStep / fTotalRange};
            return fHeight * percentage;
        }

        return 0;
    }

    static constexpr float BarOffsetY(Size size, ScrollBar::Range range, float value, uint32_t step)
    {
        const uint32_t totalRange{range.end - range.start};
        const auto percentage{(totalRange > 0) ? value / static_cast<float>(totalRange) : 0.0f};
        const auto heightLeft{static_cast<float>(size.height) - BarHeight(size, range, step)};
        return heightLeft * percentage;
    }

    void ScrollBar::onDraw(Canvas* canvas)
    {
        PTK_ASSERT(m_range.start <= m_value, "Value is out of range");
        PTK_ASSERT(m_value <= m_range.end, "Value is out of range");

        const auto size{getSize()};
        const auto rPos{getPosition()};
        const auto offset{BarOffsetY(size, m_range, m_value, m_pageStep)};

        const Vec2f vPos{static_cast<float>(rPos.x), static_cast<float>(rPos.y) + offset};
        const Vec2f vSize{static_cast<float>(size.width), BarHeight(size, m_range, m_pageStep)};

        canvas->drawRoundRect(vPos, vSize, Color::MakeRGB(100, 200, 100), 2.0f);
    }

    template <typename T>
    static constexpr T ToRangeScale(T value, Size size, ScrollBar::Range range, uint32_t step) noexcept
    {
        const auto fTotalRange{static_cast<float>(range.end - range.start)};
        const auto fHeight{static_cast<float>(size.height) - BarHeight(size, range, step)};
        const auto fValue{static_cast<float>(value)};

        if (fTotalRange != 0.0f && fHeight != 0.0f && fValue != 0.0f)
        {
            const auto percentage{fTotalRange / fHeight};
            return static_cast<T>(fValue * percentage);
        }

        return T{};
    }

    static constexpr float RangePerPixel(Size size, ScrollBar::Range range, uint32_t step) noexcept
    {
        const auto fTotalRange{static_cast<float>(range.end - range.start)};
        const auto fHeight{static_cast<float>(size.height) - BarHeight(size, range, step)};

        if (fTotalRange > 0.0f && fHeight > 0.0f)
            return fTotalRange / fHeight;

        return 0.0f;
    }

    void ScrollBar::onHoverEvent(const MotionEvent& evt)
    {
        if (m_clicked)
        {
            const auto fPoint{static_cast<float>(evt.pos.y) - static_cast<float>(getPosition().y) - m_dragOffset};
            auto fValue{ToRangeScale(fPoint, getSize(), m_range, m_pageStep)};

            const auto fStart{static_cast<float>(m_range.start)};
            const auto fEnd{static_cast<float>(m_range.end)};

            if (fStart > fValue)
                fValue = fStart;
            if (fValue > fEnd)
                fValue = fEnd;

            setPreciseValue(fValue);
        }
    }

    void ScrollBar::onEnterEvent(const EnterEvent& evt)
    {
        PTK_INFO("ScrollBar::onEnterEvent");
    }

    void ScrollBar::onLeaveEvent(const LeaveEvent& evt)
    {
        PTK_INFO("ScrollBar::onLeaveEvent");
    }

    void ScrollBar::onLeaveClickEvent(const LeaveClickEvent& evt)
    {
        PTK_INFO("ScrollBar::onLeaveClickEvent");
    }

    void ScrollBar::onClickEvent(const ClickEvent& evt)
    {
        PTK_INFO("ScrollBar::onClickEvent");

        Point::value_type pos{getPosition().y};
        pos += static_cast<Point::value_type>(BarOffsetY(getSize(), m_range, m_value, m_pageStep));
        const auto barHeight = BarHeight(getSize(), m_range, m_pageStep);
        const auto half = static_cast<Point::value_type>(barHeight / 2.0f);
        pos += half;

        const auto dist = static_cast<uint32_t>(ToRangeScale(std::abs(evt.pos.y - pos), getSize(), m_range, m_pageStep));
        const auto offset = std::min(dist, m_pageStep);
        PTK_INFO("dist: {}, pageStep: {}, offset: {}", dist, m_pageStep, offset);

        const auto value = static_cast<uint32_t>(m_value);

        if (evt.pos.y > (pos + half))
            setValue(std::min(m_range.end, value + offset));
        else if (evt.pos.y < (pos - half))
        {
            const uint32_t v = (value > offset) ? offset : value;
            setValue(std::max(m_range.start, value - v));
        }
        else
        {
            m_clicked = true;
            const auto barPos{static_cast<float>(getPosition().y) + BarOffsetY(getSize(), m_range, m_value, m_pageStep)};
            m_dragOffset = static_cast<float>(evt.pos.y) - barPos;
        }
    }

    void ScrollBar::onReleaseEvent(const ReleaseEvent& evt)
    {
        PTK_INFO("ScrollBar::onReleaseEvent");
        m_clicked = false;
    }
}