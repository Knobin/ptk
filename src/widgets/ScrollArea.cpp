//
//  widgets/ScrollArea.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-26.
//

// Local Headers
#include "../include/Assert.hpp"

// pTK Headers
#include "ptk/util/Math.hpp"
#include "ptk/widgets/ScrollArea.hpp"

// Skia Headers
PTK_DISABLE_WARN_BEGIN()
#include "include/core/SkCanvas.h"
#include "include/core/SkPath.h"
PTK_DISABLE_WARN_END()

// C++ Headers
#include <algorithm>

namespace pTK
{
    template <typename T>
    [[nodiscard]] static bool IsInRange(T val, T min, T max) noexcept
    {
        return (val >= min && val <= max);
    }

    ///////////////////////////////////////////////////////////////////////////////

    static Point::value_type AlignChildH(Widget* child, const Size& parentSize, const Size& childSize)
    {
        Point::value_type posX{0};

        // Total size including margin.
        Size cSize{child->calcOuterFromSize(childSize)};

        // Align.
        const std::underlying_type<Align>::type cAlign{child->getAlign()};
        if (IsAlignSet(cAlign, Align::Right))
            posX = static_cast<Point::value_type>(parentSize.width - cSize.width);
        else if (IsAlignSet(cAlign, Align::Center) || IsAlignSet(cAlign, Align::HCenter))
            posX = static_cast<Point::value_type>((parentSize.width / 2) - (cSize.width / 2));

        // Offset position.
        // Maybe remove "AddWithoutOverflow" since it is highly unlikely that it overflows int32.
        posX = Math::AddWithoutOverflow(posX, static_cast<Point::value_type>(child->getMargin().left));

        return posX;
    }

    ///////////////////////////////////////////////////////////////////////////////

    void ScrollArea::onScrollEvent(const ScrollEvent& evt)
    {
        performScroll(evt);
    }

    void ScrollArea::performScroll(const ScrollEvent& evt)
    {
        const float offset = getScrollOffset(evt);
        PTK_INFO("OFFSET: {}", offset);
        moveVisiblePositions(offset);

        if (offset > 0)
        {
            removeFromTail();
            addToHead();
        }
        else
        {
            removeFromHead();
            addToTail();
        }
    }

    float ScrollArea::getScrollOffset(const ScrollEvent& evt) const
    {
        if (count() == 0)
            return 0; // Can not scroll if no children are present.

        if (m_endIndex - m_startIndex == count())
            return 0; // Can not scroll if all children are visible.

        if (m_endIndex - m_startIndex == 0)
            return 0; // Can not scroll if there are no children visible.

        // Position of ScrollArea.
        Point pos{getPosition()};

        const float scrollMultiplier = 35.0f;
        float offset = evt.offset.y * scrollMultiplier;

        if (offset > 0)
        {
            // First visible widget.
            Widget* first{getFirstVisible()};
            if (first)
            {
                const auto top{first->getPosition().y - static_cast<Point::value_type>(first->getMarginTop())};

                const auto start{pos.y};

                if (m_startIndex == 0)
                {
                    if (top == start)
                        return 0; // Can not scroll above first widget in container.

                    const auto dist{static_cast<float>(start - top)};
                    return std::min(offset, dist); // Normal scroll or scroll to top.
                }

                // Check remaining size here.
                auto remaining{static_cast<float>(start - top)};
                if (remaining > offset)
                    return offset; // Offset is fine here, more size available.

                // Check tail for remaining.
                for (size_type i{m_startIndex}; i > 0; --i)
                {
                    const std::shared_ptr<Widget>& child{at(i - 1)};
                    remaining += static_cast<float>(child->getOuterSize().height);
                    if (remaining > offset)
                        return offset; // Offset is fine here, more size available.
                }

                // Remaining size here does not fit the offset.
                return remaining;
            }
        }
        else
        {
            // Last visible widget.
            Widget* last{getLastVisible()};
            if (last)
            {
                auto bottom{last->getPosition().y};
                bottom += static_cast<Point::value_type>(last->getSize().height + last->getMarginBottom());

                const auto end{pos.y + static_cast<Point::value_type>(getSize().height)};

                if (m_endIndex == count())
                {
                    if (bottom == end)
                        return 0; // Can not scroll past last widget in container.

                    const auto dist{static_cast<float>(end - bottom)};
                    return std::max(offset, dist); // Normal scroll or scroll to end.
                }

                // Check remaining size here.
                auto remaining{static_cast<float>(end - bottom)};
                if (remaining > offset)
                    return offset; // Offset is fine here, more size available.

                // Check tail for remaining.
                for (size_type i{m_endIndex}; i < count(); ++i)
                {
                    const std::shared_ptr<Widget>& child{at(i)};
                    remaining += static_cast<float>(child->getOuterSize().height);
                    if (remaining > offset)
                        return offset; // Offset is fine here, more size available.
                }

                // Remaining size here does not fit the offset.
                return remaining;
            }
        }

        return offset;
    }

    void ScrollArea::moveVisiblePositions(float offset)
    {
        // Move currently displayed widgets by offset.
        for (size_type i{m_startIndex}; i < m_endIndex; ++i)
        {
            std::shared_ptr<Widget>& child{at(i)};
            Point cPos{child->getPosition()};
            cPos.y += static_cast<Point::value_type>(offset);
            child->setPosHint(cPos);
        }
    }

    void ScrollArea::onDraw(Canvas* canvas)
    {
        const auto pos{getPosition()};
        const auto size{getSize()};

        const Vec2f fPos{static_cast<float>(pos.x), static_cast<float>(pos.y)};
        const Vec2f fSize{static_cast<float>(size.width), static_cast<float>(size.height)};

        SkPoint skPos{fPos.x, fPos.y};
        SkPoint skSize{fSize.x, fSize.y};
        skSize += skPos; // skia needs the size to be pos+size.

        SkRect clipRect{};
        clipRect.set(skPos, skSize);

        drawBackground(canvas);
        SkCanvas* skCanvas = canvas->skCanvas;
        skCanvas->save();
        skCanvas->clipRect(clipRect, SkClipOp::kIntersect, true);
        for (size_type i{m_startIndex}; i < m_endIndex; ++i)
            at(i)->onDraw(canvas);
        skCanvas->restore();
    }

    void ScrollArea::onAdd(const std::shared_ptr<Widget>& widget)
    {
        addToContent(widget);
    }

    void ScrollArea::onRemove(const std::shared_ptr<Widget>& widget)
    {
        removeFromContent(widget);
    }

    void ScrollArea::onClear()
    {
        clearContent();
    }

    void ScrollArea::addToContent(const std::shared_ptr<Widget>&)
    {
        // Widgets should already be positioned here.
        addToTail(); // Try to add widgets from tail.
    }

    void ScrollArea::removeFromContent(const std::shared_ptr<Widget>&)
    {
        adjustVisiblePositions(); // Re-adjust position of children.
        addToTail();              // Add any "hidden" children to content if possible.
    }

    void ScrollArea::addToHead()
    {
        if (count() == 0)
            return; // No point trying to add widgets that does not exist.

        // Position of ScrollArea.
        Point pos{getPosition()};

        // Last visible widget.
        Widget* first{getFirstVisible()};
        const auto offset{(first) ? first->getPosition().y - static_cast<Point::value_type>(first->getMarginTop()) : 0};

        // Range available for widgets to populate.
        const auto min{pos.y};
        auto max{offset};

        // Check if any content is available.
        if (min > max)
            return;

        for (size_type i{m_startIndex}; i > 0; --i)
        {
            std::shared_ptr<Widget>& child{at(i - 1)};
            Point cPos{pos.x, max - static_cast<Point::value_type>(child->getMarginBottom())};

            if (IsInRange(cPos.y, min, max))
            {
                cPos.x += AlignChildH(child.get(), getSize(), child->getSize());
                cPos.y -= static_cast<Point::value_type>(child->getSize().height);
                child->setPosHint(cPos);
                child->show();
                --m_startIndex;

                // Remove child size from position.
                PTK_INFO("Showing widget (addToHead), \"{}\"", child->getName());
                max -= static_cast<Point::value_type>(child->getOuterSize().height);
                if (max < min)
                    break; // Child is taking up all content, no more widgets will fit after this.
            }
            else
            {
                // No more widgets after this one will not be visible either.
                break;
            }
        }
    }

    void ScrollArea::addToTail()
    {
        if (count() == 0)
            return; // No point trying to add widgets that does not exist.

        // Position of ScrollArea.
        Point pos{getPosition()};

        // Last visible widget.
        Widget* last{getLastVisible()};
        Point::value_type offset{pos.y};
        if (last)
        {
            offset = last->getPosition().y;
            offset += static_cast<Point::value_type>(last->getSize().height + last->getMarginBottom());
        }

        // Range available for widgets to populate.
        auto min{offset};
        const auto max{pos.y + static_cast<Point::value_type>(getSize().height)};

        // Check if any content is available.
        if (min > max)
            return;

        for (size_type i{m_endIndex}; i < count(); ++i)
        {
            std::shared_ptr<Widget>& child{at(i)};
            Point cPos{pos.x, min + static_cast<Point::value_type>(child->getMarginTop())};

            if (IsInRange(cPos.y, min, max))
            {
                cPos.x += AlignChildH(child.get(), getSize(), child->getSize());
                child->setPosHint(cPos);
                child->show();
                ++m_endIndex;

                // Add child size to position.
                min += static_cast<Point::value_type>(child->getOuterSize().height);
                PTK_INFO("Showing widget (addToTail), \"{}\"", child->getName());
                if (min > max)
                    break; // Child is taking up all content, no more widgets will fit after this.
            }
            else
            {
                // No more widgets after this one will not be visible either.
                break;
            }
        }
    }

    void ScrollArea::removeFromHead()
    {
        if (count() == 0)
            return; // No point trying to add widgets that does not exist.

        // Position of ScrollArea.
        Point pos{getPosition()};

        // Start of content.
        const auto start{pos.y};

        // Hide widgets if necessary.
        for (size_type i{m_startIndex}; i < m_endIndex; ++i)
        {
            std::shared_ptr<Widget>& child{at(i)};
            const auto end{child->getPosition().y + static_cast<Point::value_type>(child->getSize().height)};
            if (start > end)
            {
                // Child is not visible here.
                ++m_startIndex;
                PTK_INFO("Hiding widget (removeFromHead), \"{}\"", child->getName());
            }
            else
            {
                // If this one is visible the others after it will also be.
                break;
            }
        }
    }

    void ScrollArea::removeFromTail()
    {
        if (count() == 0)
            return; // No point trying to add widgets that does not exist.

        // Position of ScrollArea.
        Point pos{getPosition()};

        // End of content.
        const auto end{pos.y + static_cast<Point::value_type>(getSize().height)};

        // Hide widgets if necessary.
        for (size_type i{m_endIndex}; i > m_startIndex; --i)
        {
            std::shared_ptr<Widget>& child{at(i - 1)};
            const auto y{child->getPosition().y};
            if (y > end)
            {
                // Child is not visible here.
                --m_endIndex;
                PTK_INFO("Hiding widget (removeFromTail), \"{}\"", child->getName());
            }
            else
            {
                // If this one is visible the others before it will also be.
                break;
            }
        }
    }

    Widget* ScrollArea::getFirstVisible() const
    {
        if (m_startIndex == m_endIndex)
            return nullptr; // No visible item.

        PTK_ASSERT(m_startIndex < m_endIndex, "Start index larger than end index");
        PTK_ASSERT(count() >= m_endIndex, "End index larger than count()");
        PTK_ASSERT(m_startIndex < count(), "Start index larger than count()");

        return at(m_startIndex).get();
    }

    Widget* ScrollArea::getLastVisible() const
    {
        if (m_startIndex == m_endIndex)
            return nullptr; // No visible item.

        PTK_ASSERT(m_startIndex < m_endIndex, "Start index larger than end index");
        PTK_ASSERT(count() >= m_endIndex, "End index larger than count()");
        PTK_ASSERT(m_startIndex < count(), "Start index larger than count()");

        return at(m_endIndex - 1).get();
    }

    void ScrollArea::adjustVisiblePositions()
    {
        // Re-position currently displayed widgets.
        Point pos{getPosition()};
        for (size_type i{m_startIndex}; i < m_endIndex; ++i)
        {
            std::shared_ptr<Widget>& child{at(i)};
            Point cPos{child->getPosition()};
            cPos.y = pos.y + static_cast<Point::value_type>(child->getMarginTop());
            child->setPosHint(cPos);
            pos.y += static_cast<Point::value_type>(child->getOuterSize().height);
        }
    }

    void ScrollArea::clearContent()
    {
        m_startIndex = 0;
        m_endIndex = 0;

        // TODO(knobin): Maybe set "visible" status on children to hidden if they are in the content?
    }

    void ScrollArea::onChildUpdate(size_type)
    {
        draw();
    }

    void ScrollArea::onChildDraw(size_type)
    {
        draw();
    }

    void ScrollArea::onSizeChange(const Size& size)
    {
        performSizeChanged(size);
    }

    void ScrollArea::performSizeChanged(const Size&)
    {
        // "Stick" last child to the bottom when resizing.
        if (m_startIndex != m_endIndex && m_endIndex == count())
        {
            if (m_startIndex != 0)
            {
                // Last visible widget.
                Widget* last{getLastVisible()};
                if (last)
                {
                    auto bottom{last->getPosition().y};
                    bottom += static_cast<Point::value_type>(last->getSize().height + last->getMarginBottom());

                    const auto end{getPosition().y + static_cast<Point::value_type>(getSize().height)};

                    if (end > bottom)
                    {
                        const auto offset{static_cast<float>(end - bottom)};
                        moveVisiblePositions(offset);
                    }
                }
            }
        }
        else
        {
            adjustVisiblePositions(); // Re-adjust position of children.
        }

        removeFromTail();
        addToHead();

        removeFromHead();
        addToTail();
    }
} // namespace pTK