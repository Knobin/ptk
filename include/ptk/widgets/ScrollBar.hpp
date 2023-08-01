//
//  widgets/ScrollBar.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-07-01.
//

#ifndef PTK_WIDGETS_SCROLLBAR_HPP
#define PTK_WIDGETS_SCROLLBAR_HPP

// pTK Headers
#include "ptk/core/Widget.hpp"

// TODO(knobin): Add documentation.

//
// TODO(knobin): Current way of tracking movement of the bar may result in unexpected behaviour.
// More testing is required.
//

namespace pTK
{
    class PTK_API ScrollBar : public Widget
    {
    public:
        struct Range
        {
            uint32_t start{};
            uint32_t end{};
        };

        struct OnValueChange
        {
            uint32_t value{};
            int32_t offset{};
        };

    public:
        ScrollBar() = default;
        explicit ScrollBar(Range range);
        ScrollBar(Range range, uint32_t value);
        ~ScrollBar() override = default;

        void setRange(uint32_t start, uint32_t end);
        void setRange(Range range);
        [[nodiscard]] Range range() const noexcept { return m_range; }

        [[nodiscard]] uint32_t value() const noexcept { return static_cast<uint32_t>(m_value); }
        void setValue(uint32_t value);
        void updateValue(uint32_t value);

        [[nodiscard]] uint32_t pageStep() const noexcept { return m_pageStep; }
        void setPageStep(uint32_t step);

        uint64_t onValueChange(const std::function<bool(const OnValueChange&)>& func);

        void onDraw(Canvas* canvas) override;

    public:
        void onHoverEvent(const MotionEvent& evt) override;
        void onClickEvent(const ClickEvent& evt) override;
        void onReleaseEvent(const ReleaseEvent& evt) override;

    private:
        void updateRange(Range range);
        void setPreciseValue(float value, bool trigger);

    private:
        Range m_range{};
        uint32_t m_pageStep{};
        uint32_t m_lastValue{};
        float m_value{};
        float m_dragOffset{};
        bool m_clicked{false};
    };
} // namespace pTK

#endif // PTK_WIDGETS_SCROLLBAR_HPP
