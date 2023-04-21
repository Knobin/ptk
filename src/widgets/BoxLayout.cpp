//
//  widgets/BoxLayout.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2023-03-25.
//

// pTK Headers
#include "ptk/widgets/BoxLayout.hpp"
#include "ptk/util/Math.hpp"

// C++ Headers
#include <array>
#include <functional>
#include <type_traits>
#include <utility>

namespace pTK
{
    ///////////////////////////////////////////////////////////////////////////////
    //                                 Align                                     //
    ///////////////////////////////////////////////////////////////////////////////

    static Point::value_type AlignChildV(Widget* child, const Size& parentSize, const Size& childSize)
    {
        Point::value_type posY{0};

        // Total size with margin and padding.
        Size cSize{child->calcOuterFromSize(childSize)};

        // Align
        std::underlying_type<Align>::type cAlign = child->getAlign();
        if (IsAlignSet(cAlign, Align::Bottom))
            posY = static_cast<Point::value_type>(parentSize.height - cSize.height);
        else if (IsAlignSet(cAlign, Align::Center) || IsAlignSet(cAlign, Align::VCenter))
            posY = static_cast<Point::value_type>((parentSize.height / 2) - (cSize.height / 2));

        // Offset position.
        // Maybe remove "AddWithoutOverflow" since it is highly unlikely that it overflows int32.
        posY = Math::AddWithoutOverflow(posY, static_cast<Point::value_type>(child->getMargin().top));

        return posY;
    }

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
    //                                Helpers                                    //
    ///////////////////////////////////////////////////////////////////////////////

    template <typename Func>
    static void LoopBasedOnDirection(const BoxLayout& box, BoxLayout::Direction direction, Func func)
    {
        if (IsForwardOrdering(direction))
            for (auto it = box.cbegin(); it != box.cend(); ++it)
                func(it);
        else
            for (auto it = box.crbegin(); it != box.crend(); ++it)
                func(it);
    }

    template <typename Func>
    static void LoopBasedOnDirection(BoxLayout& box, BoxLayout::Direction direction, Func func)
    {
        if (IsForwardOrdering(direction))
            for (auto it = box.begin(); it != box.end(); ++it)
                func(it);
        else
            for (auto it = box.rbegin(); it != box.rend(); ++it)
                func(it);
    }

    template <typename Forward, typename Reverse>
    static std::vector<Size::value_type> CalcSpaces(Size::value_type total, const BoxLayout& box, Forward fInfo,
                                                    Reverse rInfo)
    {
        const BoxLayout::size_type childrenCount{box.count()};
        const BoxLayout::size_type spaceCount{childrenCount + 1};
        std::vector<Size::value_type> spaces(spaceCount);

        if (total != 0)
        {
            if (IsForwardOrdering(box.direction()))
                fInfo(spaces, box.cbegin(), box.cend());
            else
                rInfo(spaces, box.crbegin(), box.crend());

            Size::value_type spacesToUse{0};
            for (auto value : spaces)
                if (value == 1)
                    ++spacesToUse;

            Size::value_type spaceSize{(spacesToUse != 0) ? static_cast<Size::value_type>(total) / spacesToUse : 0};
            for (auto& value : spaces)
                if (value == 1)
                    value = spaceSize;
        }

        return spaces;
    }

    ///////////////////////////////////////////////////////////////////////////////
    //                                Vertical                                   //
    ///////////////////////////////////////////////////////////////////////////////

    namespace VerticalLayout
    {
        static Size ContentMinSize(const BoxLayout::const_iterator& start, const BoxLayout::const_iterator& end)
        {
            Size contMinSize{Size::Min};
            for (auto it{start}; it != end; ++it)
            {
                const Limits limits{(*it)->getLimitsWithSizePolicy()};
                const Size minSize{(*it)->calcOuterFromSize(limits.min)};

                contMinSize.height = Math::AddWithoutOverflow(contMinSize.height, minSize.height);
                contMinSize.width = (minSize.width > contMinSize.width) ? minSize.width : contMinSize.width;
            }

            return contMinSize;
        }

        static Size ContentMaxSize(const BoxLayout::const_iterator& start, const BoxLayout::const_iterator& end)
        {
            Size contMaxSize{Size::Max};
            for (auto it{start}; it != end; ++it)
            {
                const Limits limits{(*it)->getLimitsWithSizePolicy()};
                const Size maxSize{(*it)->calcOuterFromSize(limits.max)};

                contMaxSize.height = Math::AddWithoutOverflow(contMaxSize.height, maxSize.height);
                contMaxSize.width = (maxSize.width > contMaxSize.width) ? maxSize.width : contMaxSize.width;
            }

            return contMaxSize;
        }

        template <typename Iter>
        static void CalcSpaceInfo(std::vector<Size::value_type>& spaces, Iter start, Iter end)
        {
            std::size_t i{0};
            for (auto it{start}; it != end; ++it)
            {
                const auto& child = *it;
                const std::underlying_type<Align>::type cAlign{child->getAlign()};

                if (IsAlignSet(cAlign, Align::Top))
                {
                    spaces.at(i) = 0;
                    spaces.at(i + 1) = 1;
                }
                else if (IsAlignSet(cAlign, Align::Bottom))
                {
                    if (i == 0)
                        spaces.at(i) = 1;
                    else
                        spaces.at(i) = (spaces.at(i) == 0) ? 0 : 1;

                    spaces.at(i + 1) = 0;
                }

                if (IsAlignSet(cAlign, Align::Center) || IsAlignSet(cAlign, Align::VCenter))
                {
                    spaces.at(i) = 1;
                    spaces.at(i + 1) = 1;
                }

                ++i;
            }
        }

        static void RefitContent(Size boxSize, Point boxPos, BoxLayout::Direction direction, const BoxLayout& box)
        {
            const BoxLayout::size_type childrenCount{box.count()};
            std::vector<Size> data(childrenCount);

            // Initialize sizes.
            std::size_t i{0};
            LoopBasedOnDirection(box, direction, [boxSize, &data, &i](auto it) {
                const Limits limits = (*it)->getLimitsWithSizePolicy();
                data.at(i) = (*it)->calcOuterFromSize(limits.min);

                const Size::value_type maxWidth{(*it)->calcOuterFromSize(limits.max).width};
                data.at(i).width = (boxSize.width > maxWidth) ? maxWidth : boxSize.width;

                ++i;
            });

            // Expand children to its max sizes possible.
            const Size::value_type heightLeft = boxSize.height - ContentMinSize(box.cbegin(), box.cend()).height;
            Size::value_type totalEachLeft{heightLeft};

            // Distribute heightLeft.
            // Need to fix this some time.
            // TODO: it takes many iteration before the height is distributed, especially if only 1 can grow.
            Size::value_type lastEachLeft = totalEachLeft;
            while (totalEachLeft > 0)
            {
                Size::value_type eachAdd{static_cast<Size::value_type>(
                    std::floor(static_cast<float>(totalEachLeft) / static_cast<float>(childrenCount)))};
                eachAdd = (totalEachLeft < static_cast<Size::value_type>(childrenCount)) ? 1 : eachAdd;
                bool done{true};

                i = 0;
                for (const auto& child : box)
                {
                    const Limits limits = child->getLimitsWithSizePolicy();

                    // Max - Min
                    const Size::value_type maxHeight = child->calcOuterFromSize(limits.max).height;
                    const Size::value_type delta{maxHeight - data.at(i).height};

                    if (delta > 0)
                    {
                        if (eachAdd > delta)
                        {
                            data.at(i).height += delta;
                            totalEachLeft -= (delta);
                            done = false;
                        }
                        else
                        {
                            data.at(i).height += eachAdd;
                            totalEachLeft -= (eachAdd);
                            done = false;
                        }
                    }

                    if (totalEachLeft == 0)
                    {
                        done = true;
                        break;
                    }

                    ++i;
                }

                // Check if the total was reduced.
                // if not break as nothing was added (same will happen next iteration).
                if (totalEachLeft == lastEachLeft)
                    break;

                lastEachLeft = totalEachLeft;

                // We cannot add more to the widgets.
                if ((done) || (eachAdd == 0))
                    break;
            }

            // TODO: Fix if we break in while loop (size left unused).
            using vec_type = std::vector<Size::value_type>;
            const auto fInfo = [](vec_type& spaces, BoxLayout::const_iterator start, BoxLayout::const_iterator end) {
                CalcSpaceInfo(spaces, std::move(start), std::move(end));
            };
            const auto rInfo =
                [](vec_type& spaces, BoxLayout::const_reverse_iterator start, BoxLayout::const_reverse_iterator end) {
                CalcSpaceInfo(spaces, std::move(start), std::move(end));
            };
            const std::vector<Size::value_type> spaces{CalcSpaces(totalEachLeft, box, fInfo, rInfo)};

            // Update size and position for widgets.
            i = 0;
            LoopBasedOnDirection(box, direction, [&boxPos, boxSize, &data, &spaces, &i](auto it) {
                constexpr int int_max = std::numeric_limits<int>::max();
                auto& child = (*it);

                // Margin.
                Margin cMargin{child->getMargin()};
                cMargin.top = (cMargin.top > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.top;
                cMargin.bottom = (cMargin.bottom > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.bottom;

                // New Size.
                data.at(i).height -= static_cast<Size::value_type>(cMargin.top + cMargin.bottom);
                data.at(i).width -= static_cast<Size::value_type>(cMargin.left + cMargin.right);
                if (child->getSize() != data.at(i))
                    child->setSize(data.at(i));

                // Position.
                boxPos.y += static_cast<Point::value_type>(cMargin.top) + static_cast<Point::value_type>(spaces.at(i));
                child->setPosHint(Point(boxPos.x + AlignChildH(child.get(), boxSize, data.at(i)), boxPos.y));
                boxPos.y +=
                    static_cast<Point::value_type>(data.at(i).height) + static_cast<Point::value_type>(cMargin.bottom);

                ++i;
            });
        }

        template <typename Iter>
        static void ExpandOnAddSetter(Size size, Point pos, Iter start, Iter end)
        {
            constexpr int int_max = std::numeric_limits<int>::max();

            for (auto it{start}; it != end; ++it)
            {
                auto& child = (*it);

                Size cSize = child->getMinSize();
                if (child->getSize() != cSize)
                    child->setSize(cSize);

                Margin cMargin{child->getMargin()};
                cMargin.top = (cMargin.top > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.top;
                cMargin.bottom = (cMargin.bottom > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.bottom;

                pos.y += static_cast<Point::value_type>(cMargin.top);
                child->setPosHint(Point(pos.x + AlignChildH(child.get(), size, cSize), pos.y));
                pos.y += static_cast<Point::value_type>(cSize.height) + static_cast<Point::value_type>(cMargin.bottom);
            }
        }

        static void ExpandOnAdd(Size size, Point pos, BoxLayout::Direction direction, BoxLayout& box)
        {
            /** We should not have to consider auto margin for children.
                This function should only be called when adding and children wont fit in current
                size and we only expand so the they will fit with their size and margin.
                Auto margin is not included.
            */

            if (direction == BoxLayout::Direction::TopToBottom)
                ExpandOnAddSetter(size, pos, box.begin(), box.end());
            else
                ExpandOnAddSetter(size, pos, box.rbegin(), box.rend());
        }
    } // namespace VerticalLayout

    ///////////////////////////////////////////////////////////////////////////////
    //                               Horizontal                                  //
    ///////////////////////////////////////////////////////////////////////////////

    namespace HorizontalLayout
    {
        static Size ContentMinSize(const BoxLayout::const_iterator& start, const BoxLayout::const_iterator& end)
        {
            Size contMinSize{Size::Min};
            for (auto it{start}; it != end; ++it)
            {
                const Limits limits{(*it)->getLimitsWithSizePolicy()};
                const Size minSize{(*it)->calcOuterFromSize(limits.min)};

                contMinSize.height = (minSize.height > contMinSize.height) ? minSize.height : contMinSize.height;
                contMinSize.width = Math::AddWithoutOverflow(contMinSize.width, minSize.width);
            }

            return contMinSize;
        }

        static Size ContentMaxSize(const BoxLayout::const_iterator& start, const BoxLayout::const_iterator& end)
        {
            Size contMaxSize{Size::Max};
            for (auto it{start}; it != end; ++it)
            {
                const Limits limits{(*it)->getLimitsWithSizePolicy()};
                const Size maxSize{(*it)->calcOuterFromSize(limits.max)};

                contMaxSize.height = (maxSize.height > contMaxSize.height) ? maxSize.height : contMaxSize.height;
                contMaxSize.width = Math::AddWithoutOverflow(contMaxSize.width, maxSize.width);
            }

            return contMaxSize;
        }

        template <typename Iter>
        static void CalcSpaceInfo(std::vector<Size::value_type>& spaces, Iter start, Iter end)
        {
            std::size_t i{0};
            for (auto it{start}; it != end; ++it)
            {
                const auto& child = *it;
                const std::underlying_type<Align>::type cAlign{child->getAlign()};

                if (IsAlignSet(cAlign, Align::Left))
                {
                    spaces.at(i) = 0;
                    spaces.at(i + 1) = 1;
                }
                else if (IsAlignSet(cAlign, Align::Right))
                {
                    if (i == 0)
                        spaces.at(i) = 1;
                    else
                        spaces.at(i) = (spaces.at(i) == 0) ? 0 : 1;

                    spaces.at(i + 1) = 0;
                }

                if (IsAlignSet(cAlign, Align::Center) || IsAlignSet(cAlign, Align::HCenter))
                {
                    spaces.at(i) = 1;
                    spaces.at(i + 1) = 1;
                }

                ++i;
            }
        }

        static void RefitContent(Size boxSize, Point boxPos, BoxLayout::Direction direction, const BoxLayout& box)
        {
            const BoxLayout::size_type childrenCount{box.count()};
            std::vector<Size> data(childrenCount);

            // Initialize sizes.
            std::size_t i{0};
            LoopBasedOnDirection(box, direction, [boxSize, &data, &i](auto it) {
                const Limits limits = (*it)->getLimitsWithSizePolicy();
                data.at(i) = (*it)->calcOuterFromSize(limits.min);

                const Size::value_type maxHeight{(*it)->calcOuterFromSize(limits.max).height};
                data.at(i).height = (boxSize.height > maxHeight) ? maxHeight : boxSize.height;

                ++i;
            });

            // Expand children to its max sizes possible.
            const Size::value_type widthLeft{boxSize.width - ContentMinSize(box.cbegin(), box.cend()).width};
            Size::value_type totalEachLeft{widthLeft};

            // Distribute heightLeft.
            // Need to fix this some time.
            // TODO: it takes many iteration before the height is distributed, especially if only 1 can grow.
            Size::value_type lastEachLeft = totalEachLeft;
            while (totalEachLeft > 0)
            {
                Size::value_type eachAdd{static_cast<Size::value_type>(
                    std::floor(static_cast<float>(totalEachLeft) / static_cast<float>(childrenCount)))};
                eachAdd = (totalEachLeft < static_cast<Size::value_type>(childrenCount)) ? 1 : eachAdd;
                bool done{true};

                i = 0;
                for (const auto& child : box)
                {
                    const Limits limits = child->getLimitsWithSizePolicy();

                    // Max - Min
                    const Size::value_type maxWidth = child->calcOuterFromSize(limits.max).width;
                    const Size::value_type delta{maxWidth - data.at(i).width};

                    if (delta > 0)
                    {
                        if (eachAdd > delta)
                        {
                            data.at(i).width += delta;
                            totalEachLeft -= (delta);
                            done = false;
                        }
                        else
                        {
                            data.at(i).width += eachAdd;
                            totalEachLeft -= (eachAdd);
                            done = false;
                        }
                    }

                    if (totalEachLeft == 0)
                    {
                        done = true;
                        break;
                    }

                    ++i;
                }

                // Check if the total was reduced.
                // if not break as nothing was added (same will happen next iteration).
                if (totalEachLeft == lastEachLeft)
                    break;

                lastEachLeft = totalEachLeft;

                // We cannot add more to the widgets.
                if ((done) || (eachAdd == 0))
                    break;
            }

            // TODO: Fix if we break in while loop (size left unused).
            using vec_type = std::vector<Size::value_type>;
            const auto fInfo = [](vec_type& spaces, BoxLayout::const_iterator start, BoxLayout::const_iterator end) {
                CalcSpaceInfo(spaces, std::move(start), std::move(end));
            };
            const auto rInfo =
                [](vec_type& spaces, BoxLayout::const_reverse_iterator start, BoxLayout::const_reverse_iterator end) {
                CalcSpaceInfo(spaces, std::move(start), std::move(end));
            };
            const std::vector<Size::value_type> spaces{CalcSpaces(totalEachLeft, box, fInfo, rInfo)};

            // Update size and position for widgets.
            i = 0;
            LoopBasedOnDirection(box, direction, [&boxPos, boxSize, &data, &spaces, &i](auto it) {
                constexpr int int_max = std::numeric_limits<int>::max();
                auto& child = (*it);

                // Margin.
                Margin cMargin{child->getMargin()};
                cMargin.left = (cMargin.left > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.left;
                cMargin.right = (cMargin.right > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.right;

                // New Size.
                data.at(i).height -= static_cast<Size::value_type>(cMargin.top + cMargin.bottom);
                data.at(i).width -= static_cast<Size::value_type>(cMargin.left + cMargin.right);
                if (child->getSize() != data.at(i))
                    child->setSize(data.at(i));

                // Position.
                boxPos.x += static_cast<Point::value_type>(cMargin.left) + static_cast<Point::value_type>(spaces.at(i));
                child->setPosHint(Point(boxPos.x, boxPos.y + AlignChildV(child.get(), boxSize, data.at(i))));
                boxPos.x +=
                    static_cast<Point::value_type>(data.at(i).width) + static_cast<Point::value_type>(cMargin.right);

                ++i;
            });
        }

        template <typename Iter>
        static void ExpandOnAddSetter(Size size, Point pos, Iter start, Iter end)
        {
            constexpr int int_max = std::numeric_limits<int>::max();

            for (auto it{start}; it != end; ++it)
            {
                auto& child = (*it);

                Size cSize = child->getMinSize();
                if (child->getSize() != cSize)
                    child->setSize(cSize);

                Margin cMargin{child->getMargin()};
                cMargin.left = (cMargin.left > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.left;
                cMargin.right = (cMargin.right > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.right;

                pos.x += static_cast<Point::value_type>(cMargin.left);
                child->setPosHint(Point(pos.x, pos.y + AlignChildV(child.get(), size, cSize)));
                pos.x += static_cast<Point::value_type>(cSize.width) + static_cast<Point::value_type>(cMargin.right);
            }
        }

        static void ExpandOnAdd(Size size, Point pos, BoxLayout::Direction direction, BoxLayout& box)
        {
            /** We should not have to consider auto margin for children.
                This function should only be called when adding and children wont fit in current
                size and we only expand so the they will fit with their size and margin.
                Auto margin is not included.
            */

            if (direction == BoxLayout::Direction::LeftToRight)
                ExpandOnAddSetter(size, pos, box.begin(), box.end());
            else
                ExpandOnAddSetter(size, pos, box.rbegin(), box.rend());
        }
    } // namespace HorizontalLayout

    ///////////////////////////////////////////////////////////////////////////////
    //                               BoxLayout                                   //
    ///////////////////////////////////////////////////////////////////////////////

    BoxLayout::BoxLayout(Direction direction)
        : WidgetContainer(),
          m_direction{direction}
    {
        setMaxSize(Size::Max);
    }

    void BoxLayout::refitContent(Size size, Point pos)
    {
        const size_type childrenCount{count()};
        if (childrenCount == 0)
            return;

        if (IsVerticalOrdering(direction()))
            VerticalLayout::RefitContent(size, pos, direction(), *this);
        else
            HorizontalLayout::RefitContent(size, pos, direction(), *this);
    }

    void BoxLayout::expandOnAdd(Size size, Point pos)
    {
        Size layoutSize{getSize()};
        layoutSize.height = (size.height > layoutSize.height) ? size.height : layoutSize.height;
        layoutSize.width = (size.width > layoutSize.width) ? size.width : layoutSize.width;
        setSize(layoutSize); // this will generate a Resize event.

        if (IsVerticalOrdering(direction()))
            VerticalLayout::ExpandOnAdd(size, pos, direction(), *this);
        else
            HorizontalLayout::ExpandOnAdd(size, pos, direction(), *this);
    }

    bool BoxLayout::onLayoutRequest(Direction)
    {
        return true;
    }

    void BoxLayout::onLayoutChange()
    {
        const Size minLayoutSize{calcMinSize()};
        setMinSize(minLayoutSize);
        const Size vbSize{getSize()};

        if ((minLayoutSize.width > vbSize.width) || (minLayoutSize.height > vbSize.height))
        {
            // Children will not fit in the current size.
            // Set minimal size to HBox and set minimal size to each child.
            expandOnAdd(minLayoutSize, getPosition());
        }
        else
        {
            // Children will fit in the current size.
            // Only need to resize and position children.
            refitContent(vbSize, getPosition());
        }
    }

    static void ForwardDraw(BoxLayout* box, Canvas* canvas)
    {
        for (auto it{box->begin()}; it != box->end(); ++it)
            (*it)->onDraw(canvas);
    }

    static void ReverseDraw(BoxLayout* box, Canvas* canvas)
    {
        for (auto it{box->rbegin()}; it != box->rend(); ++it)
            (*it)->onDraw(canvas);
    }

    void BoxLayout::drawChildrenWithDir(Canvas* canvas)
    {
        constexpr auto lookup = std::array{ForwardDraw, ReverseDraw, ForwardDraw, ReverseDraw};

        using dir_utype = std::underlying_type_t<Direction>;
        lookup[static_cast<decltype(lookup)::size_type>(static_cast<dir_utype>(direction()))](this, canvas);
    }

    void BoxLayout::onDraw(Canvas* canvas)
    {
        drawBackground(canvas);
        drawChildrenWithDir(canvas);
    }

    void BoxLayout::setDirection(Direction direction)
    {
        if (onLayoutRequest(direction))
        {
            m_direction = direction;
            onLayoutChange();
            triggerEvent<Direction>(direction);
        }
    }

    void BoxLayout::updateDirection(Direction direction)
    {
        m_direction = direction;
    }

    void BoxLayout::onAdd(const value_type&)
    {
        const Size minLayoutSize{calcMinSize()};
        setMinSize(minLayoutSize);
        const Size vbSize{getSize()};

        if ((minLayoutSize.width > vbSize.width) || (minLayoutSize.height > vbSize.height))
        {
            // Children will not fit in the current size.
            // Set minimal size to HBox and set minimal size to each child.
            expandOnAdd(minLayoutSize, getPosition());
        }
        else
        {
            // Children will fit in the current size.
            // Only need to resize and position children.
            refitContent(vbSize, getPosition());
        }
    }

    void BoxLayout::onRemove(const value_type&)
    {
        refitContent(getSize(), getPosition());
    }

    void BoxLayout::onChildUpdate(size_type)
    {
        refitContent(getSize(), getPosition());
    }

    void BoxLayout::onSizeChange(const Size& size)
    {
        refitContent(size, getPosition());
    }

    Size BoxLayout::calcMinSize() const
    {
        if (IsVerticalOrdering(direction()))
            return calcOuterFromSize(VerticalLayout::ContentMinSize(cbegin(), cend()));

        return calcOuterFromSize(HorizontalLayout::ContentMinSize(cbegin(), cend()));
    }

    Size BoxLayout::calcMaxSize() const
    {
        if (IsVerticalOrdering(direction()))
            return calcOuterFromSize(VerticalLayout::ContentMaxSize(cbegin(), cend()));

        return calcOuterFromSize(HorizontalLayout::ContentMaxSize(cbegin(), cend()));
    }
} // namespace pTK
