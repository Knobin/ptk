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
#include "ptk/widgets/ScrollBar.hpp"

// C++ Headers
#include <memory>

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
        ScrollArea();
        ~ScrollArea() override = default;

        void onDraw(Canvas* canvas) override;

        [[nodiscard]] bool isValidOffset(int32_t offset) const;

    private:
        void onScrollEvent(const ScrollEvent& evt) override;


        void performScroll(int32_t offset);
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

        [[nodiscard]] float getScrollOffset(float eventOffset, float scrollMultiplier = 35.0f) const;
        void setScrollBarAttributes();

        [[nodiscard]] Widget* getFirstVisible() const;
        [[nodiscard]] Widget* getLastVisible() const;

        [[nodiscard]] uint32_t getContentHeight() const;
        [[nodiscard]] uint32_t getContentWidth() const;

        void onChildUpdate(size_type) override;
        void onChildDraw(size_type) override;
        void onSizeChange(const Size& size) override;

    protected:
        /** Function callback for KeyEvent.

            @param evt     KeyEvent
        */
        void onKeyEvent(const KeyEvent& evt) override;

        /** Function callback for MotionEvent.

            @param evt     MotionEvent
        */
        void onHoverEvent(const MotionEvent& evt) override;

        /** Function for handling when mouse is entering.

        */
        void onEnterEvent(const EnterEvent& evt) override;

        /** Function callback for LeaveEvent.

            @param evt     LeaveEvent
        */
        void onLeaveEvent(const LeaveEvent& evt) override;

        /** Function callback for LeaveClickEvent.

            @param evt     LeaveClickEvent
        */
        void onLeaveClickEvent(const LeaveClickEvent& evt) override;

        /** Function callback for ClickEvent.

            @param evt     ClickEvent
        */
        void onClickEvent(const ClickEvent& evt) override;

        /** Function callback for ReleaseEvent.

            @param evt     ReleaseEvent
        */
        void onReleaseEvent(const ReleaseEvent& evt) override;

    private:
        std::shared_ptr<ScrollBar> m_scrollBar{nullptr};
        size_type m_startIndex{0};
        size_type m_endIndex{0};
        uint64_t m_scrollBarCallbackId{};
        bool m_scrollBarActive{false};
        bool m_scrollBarClicked{false};
    };
}

#endif // PTK_WIDGETS_SCROLL_AREA_HPP
