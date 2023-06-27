//
//  widgets/ScrollArea.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-26.
//

// Local Headers
#include "../include/Assert.hpp"

// pTK Headers
#include "ptk/core/Widget.hpp"
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
        posX = Math::AddWithoutOverflow(posX, static_cast<pTK::Point::value_type>(child->getMargin().left));

        return posX;
    }

    ///////////////////////////////////////////////////////////////////////////////

    void ScrollArea::onScrollEvent(const ScrollEvent& evt)
    {
        performScroll(evt);
    }

    void ScrollArea::performScroll(const ScrollEvent& evt)
    {
        // TODO(knobin): Add scrolling functionality (adding and removing from head and tail).
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

    void ScrollArea::onAdd(const std::shared_ptr<pTK::Widget>& widget)
    {
        addToContent(widget);
    }

    void ScrollArea::onRemove(const std::shared_ptr<pTK::Widget>& widget)
    {
        removeFromContent(widget);
    }

    void ScrollArea::onClear()
    {
        clearContent();
    }

    void ScrollArea::addToContent(const std::shared_ptr<pTK::Widget>& widget)
    {
        // Widgets should already be positioned here.
        addFromTail(); // Try to add widgets from tail.
    }

    void ScrollArea::removeFromContent(const std::shared_ptr<pTK::Widget>&)
    {
        adjustVisiblePositions(); // Re-adjust position of children.
        addFromTail(); // Add any "hidden" children to content if possible.
    }

    void ScrollArea::addFromHead()
    {
        if (count() == 0)
            return; // No point trying to add widgets that does not exist.

        // Position of ScrollArea.
        pTK::Point pos{getPosition()};

        // Last visible widget.
        Widget* first{getFirstVisible()};
        const auto firstOffset{(first) ? static_cast<pTK::Point::value_type>(first->getOuterSize().height) : 0};

        // Range available for widgets to populate.
        const auto min{pos.y - firstOffset};
        auto max{pos.y};

        // Check if any content is available.
        if (min > max)
            return;

        for (size_type i{m_startIndex}; i > 0; --i)
        {
            std::shared_ptr<Widget>& child{at(i - 1)};
            pTK::Point cPos{pos.x, max - static_cast<pTK::Point::value_type>(child->getMarginBottom())};

            if (IsInRange(cPos.y, min, max))
            {
                cPos.x += AlignChildH(child.get(), getSize(), child->getSize());
                child->setPosHint(cPos);
                child->show();
                --m_startIndex;

                // Remove child size from position.
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

    void ScrollArea::addFromTail()
    {
        if (count() == 0)
            return; // No point trying to add widgets that does not exist.

        // Position of ScrollArea.
        pTK::Point pos{getPosition()};

        // Last visible widget.
        Widget* last{getLastVisible()};
        const auto lastOffset{(last) ? static_cast<pTK::Point::value_type>(last->getOuterSize().height) : 0};

        // Range available for widgets to populate.
        auto min{pos.y + lastOffset};
        const auto max{pos.y + static_cast<pTK::Point::value_type>(getSize().height)};

        // Check if any content is available.
        if (min > max)
            return;

        for (size_type i{m_endIndex}; i < count(); ++i)
        {
            std::shared_ptr<Widget>& child{at(i)};
            pTK::Point cPos{pos.x, min + static_cast<pTK::Point::value_type>(child->getMarginTop())};

            if (IsInRange(cPos.y, min, max))
            {
                cPos.x += AlignChildH(child.get(), getSize(), child->getSize());
                child->setPosHint(cPos);
                child->show();
                ++m_endIndex;

                // Add child size to position.
                min += static_cast<Point::value_type>(child->getOuterSize().height);
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

    Widget* ScrollArea::getFirstVisible() const
    {
        if (m_startIndex == m_endIndex)
            return nullptr; // No visible item.

        PTK_ASSERT(m_startIndex < m_endIndex, "Start index larger than end index");
        PTK_ASSERT(count() > m_endIndex, "End index larger than count()");
        PTK_ASSERT(m_startIndex < count(), "Start index larger than count()");

        return at(m_startIndex).get();
    }

    Widget* ScrollArea::getLastVisible() const
    {
        if (m_startIndex == m_endIndex)
            return nullptr; // No visible item.

        PTK_ASSERT(m_startIndex < m_endIndex, "Start index larger than end index");
        PTK_ASSERT(count() > m_endIndex, "End index larger than count()");
        PTK_ASSERT(m_startIndex < count(), "Start index larger than count()");

        return at(m_endIndex - 1).get();
    }

    void ScrollArea::adjustVisiblePositions()
    {
        // Re-position currently displayed widgets.
        pTK::Point pos{getPosition()};
        for (size_type i{m_startIndex}; i < m_endIndex; ++i)
        {
            std::shared_ptr<Widget>& child{at(i)};
            pTK::Point cPos{child->getPosition()};
            cPos.y = pos.y;
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

    void ScrollArea::onSizeChange(const pTK::Size&)
    {
        adjustVisiblePositions(); // Re-adjust position of children.
        addFromTail(); // Add any "hidden" children to content if possible.
    }
} // namespace pTK