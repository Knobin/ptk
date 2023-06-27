//
//  widgets/ScrollArea.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-06-26.
//

#ifndef PTK_WIDGETS_SCROLL_AREA_HPP
#define PTK_WIDGETS_SCROLL_AREA_HPP

// pTK Headers
#include "ptk/core/Widget.hpp"
#include "ptk/core/WidgetContainer.hpp"

//
// TODO(knobin): ScrollArea should add a Direction to layout widgets in, similarly from BoxLayout.
//
// The ScrollArea can not (and should not) inherit from BoxLayout as they are significantly different and fill
// different purposes which should stay that way. Either ScrollArea should include its own enum class of Direction,
// or the Direction enum class from BoxLayout should be moved out into LayoutDirection and be universal that any
// container can use for laying out the ordering of widgets.
//

namespace pTK
{
    class PTK_API ScrollArea : public WidgetContainer
    {
    public:
        ScrollArea() = default;
        ~ScrollArea() override = default;

    private:
        void onScrollEvent(const ScrollEvent& evt) override;
        void performScroll(const ScrollEvent& evt);
        void onDraw(Canvas* canvas) override;

        void onAdd(const std::shared_ptr<pTK::Widget>& widget) override;
        void onRemove(const std::shared_ptr<pTK::Widget>& widget) override;
        void onClear() override;

        void addToContent(const std::shared_ptr<pTK::Widget>& widget);
        void removeFromContent(const std::shared_ptr<pTK::Widget>& widget);
        void clearContent();

        void addFromHead();
        void addFromTail();
        void adjustVisiblePositions();

        [[nodiscard]] Widget* getFirstVisible() const;
        [[nodiscard]] Widget* getLastVisible() const;

        void onChildUpdate(size_type) override;
        void onChildDraw(size_type) override;
        void onSizeChange(const pTK::Size&) override;

    private:
        size_type m_startIndex{0};
        size_type m_endIndex{0};
    };
}

#endif // PTK_WIDGETS_SCROLL_AREA_HPP
