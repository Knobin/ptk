//
//  widgets/ScrollArea.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-26.
//

#ifndef PTK_WIDGETS_SCROLL_AREA_HPP
#define PTK_WIDGETS_SCROLL_AREA_HPP

// pTK Headers
#include "ptk/core/WidgetContainer.hpp"

//
// TODO(knobin): ScrollArea should add a Direction to layout widgets in, similarly from BoxLayout.
//
// The ScrollArea can not (and should not) inherit from BoxLayout as they are significantly different and fill
// different purposes which should stay that way. Either ScrollArea should include its own enum class of Direction,
// or the Direction enum class from BoxLayout should be moved out into LayoutDirection and be universal that any
// container can use for laying out the ordering of widgets.
//

// TODO(knobin): Add documentation.

namespace pTK
{
    class PTK_API ScrollArea : public WidgetContainer
    {
    public:
        ScrollArea() = default;
        ~ScrollArea() override = default;

    private:
        void onScrollEvent(const ScrollEvent& evt) override;
        void onDraw(Canvas* canvas) override;

        void performScroll(const ScrollEvent& evt);
        void performSizeChanged(const Size&);

        void onAdd(const std::shared_ptr<Widget>&) override;
        void onRemove(const std::shared_ptr<Widget>&) override;
        void onClear() override;

        void addToContent(const std::shared_ptr<Widget>&);
        void removeFromContent(const std::shared_ptr<Widget>&);
        void clearContent();

        void addToHead();
        void addToTail();
        void adjustVisiblePositions();
        void moveVisiblePositions(float offset);
        void removeFromHead();
        void removeFromTail();

        [[nodiscard]] float getScrollOffset(const ScrollEvent& evt) const;

        [[nodiscard]] Widget* getFirstVisible() const;
        [[nodiscard]] Widget* getLastVisible() const;

        void onChildUpdate(size_type) override;
        void onChildDraw(size_type) override;
        void onSizeChange(const Size& size) override;

    private:
        size_type m_startIndex{0};
        size_type m_endIndex{0};
    };
}

#endif // PTK_WIDGETS_SCROLL_AREA_HPP
