//
//  widgets/HBox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-27.
//

// pTK Headers
#include "ptk/widgets/HBox.hpp"

// C++ Headers
#include <limits>

namespace pTK
{
    HBox::HBox()
        : WidgetContainer()
    {
        setMaxSize(Size::Max);
    }

    void HBox::onAdd(const Ref<Widget>&)
    {
        const Size minLayoutSize{calcMinSize()};
        setMinSize(minLayoutSize);
        const Size vbSize{getSize()};

        if ((minLayoutSize.width > vbSize.width) || (minLayoutSize.height > vbSize.height))
        {
            /** Children will not fit in the current size.

             Set minimal size to HBox and set minimal size to each child.
             */
            expandOnAdd(minLayoutSize);
        }else
        {
            /** Childs will fit in the current size.

             Only need to resize and position children.
             */
            refitContent(vbSize);
        }
    }

    void HBox::onRemove(const Ref<Widget>&)
    {
        refitContent(getSize());
    }

    void HBox::onChildUpdate(size_type)
    {
        /*
        const Size cMaxSize{calcMaxSize()};
        Size maxSize{getMaxSize()};
        if (cMaxSize > maxSize)
            maxSize = cMaxSize;

        const Size cMinSize{calcMinSize()};
        Size minSize{getMinSize()};
        if (cMinSize > minSize)
            minSize = cMinSize;

        setLimits(minSize, maxSize);
        */
    }

    void HBox::expandOnAdd(const Size& newSize)
    {
        Size layoutSize{newSize};
        const Size vbSize{getSize()};
        Point vbPos{getPosition()};

        layoutSize.height   = (vbSize.height > layoutSize.height) ? vbSize.height : layoutSize.height;
        layoutSize.width    = (vbSize.width > layoutSize.width) ? vbSize.width : layoutSize.width;

        setSize(layoutSize); // this will generate a Resize event.
        const size_type children{container().size()};
        constexpr int int_max = std::numeric_limits<int>::max();
        for (size_type i{0}; i < children; ++i)
        {
            auto child = at(i);
            Size cSize = child->getMinSize();

            /** We should not have to consider auto margin for children.
                This function should only be called when adding and children wont fit in current
                size and we only expand so the they will fit with their size and margin.
                Auto margin is not included.
             */
            if (child->getSize() != cSize)
                child->setSize(cSize);
            Margin cMargin{child->getMargin()};
            cMargin.left = (cMargin.left > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.left;
            cMargin.right = (cMargin.right > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.right;

            vbPos.x += static_cast<Point::value_type>(cMargin.left);
            child->setPosHint(Point(vbPos.x, vbPos.y + alignChildV(i, vbSize, cSize)));
            vbPos.x += static_cast<Point::value_type>(cSize.width) + static_cast<Point::value_type>(cMargin.right);
        }
    }

    void HBox::refitContent(const Size& nsize)
    {
        const size_type children{container().size()};
        if (children == 0)
            return;

        const Size vbSize{nsize};
        std::vector<Size> sizes(children);

        // Initialize sizes.
        for (size_type i{0}; i < children; ++i)
        {
            sizes.at(i) = at(i)->calcOuterFromSize(at(i)->getMinSize());

            const Size::value_type maxHeight{at(i)->calcOuterFromSize(at(i)->getMaxSize()).height};
            sizes.at(i).height = (vbSize.height > maxHeight) ? maxHeight : vbSize.height;
        }

        // Expand children to its max sizes possible.
        const Size::value_type widthLeft{vbSize.width - calcMinSize().width};
        Size::value_type totalEachLeft{widthLeft};

        // Distribute heightLeft.
        // Need to fix this some time.
        // TODO: it takes many iteration before the height is distributed, especially if only 1 can grow.
        while (totalEachLeft > 0)
        {
            Size::value_type eachAdd{static_cast<Size::value_type>(std::floor(static_cast<float>(totalEachLeft) / static_cast<float>(children)))};
            eachAdd = (totalEachLeft < static_cast<Size::value_type>(children)) ? 1 : eachAdd;
            bool done{true};
            for (size_type i{0}; i < children; ++i)
            {
                // Max - Min

                const Size::value_type maxWidth = at(i)->calcOuterFromSize(at(i)->getMaxSize()).width;
                const Size::value_type delta{maxWidth - sizes.at(i).width};

                if (delta > 0)
                {
                    if (eachAdd > delta)
                    {
                        sizes.at(i).width += delta;
                        totalEachLeft -= (delta);
                        done = false;
                    }else
                    {
                        sizes.at(i).width += eachAdd;
                        totalEachLeft -= (eachAdd);
                        done = false;
                    }
                }

                if (totalEachLeft == 0)
                {
                    done = true;
                    break;
                }
            }

            // We cannot add more to the widgets.
            if ((done) || (eachAdd == 0))
                break;
        }

        // TODO: Fix if we break in while loop (size left unused).
        const std::vector<Size::value_type> spaces{calcSpaces(totalEachLeft)};

        // Set sizes to children.
        Point vbPos{getPosition()};
        constexpr int int_max = std::numeric_limits<int>::max();
        for (size_type i{0}; i != children; i++)
        {
            auto child{at(i)};
            Size cSize{sizes.at(i)};
            Margin cMargin{child->getMargin()};
            cMargin.left = (cMargin.left > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.left;
            cMargin.right = (cMargin.right > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.right;
            cSize.height -= static_cast<Size::value_type>(cMargin.top + cMargin.bottom);
            cSize.width -= static_cast<Size::value_type>(cMargin.left + cMargin.right);
            vbPos.x += static_cast<Point::value_type>(cMargin.left) + static_cast<Point::value_type>(spaces.at(i));
            if (child->getSize() != cSize)
                child->setSize(cSize);
            child->setPosHint(Point(vbPos.x, vbPos.y + alignChildV(i, vbSize, cSize)));
            vbPos.x += static_cast<Point::value_type>(cSize.width) + static_cast<Point::value_type>(cMargin.right);
        }
    }

    void HBox::onSizeChange(const Size& size)
    {
        refitContent(size);
    }

    std::vector<Size::value_type> HBox::calcSpaces(Size::value_type width)
    {
        const size_type children{container().size()};
        const size_type spaceCount{children + 1};
        std::vector<Size::value_type> spaces(spaceCount);
        if (width != 0)
        {
            for (size_type i{0}; i != children; i++)
            {
                const std::underlying_type<Align>::type cAlign{at(i)->getAlign()};

                if (IsAlignSet(cAlign, Align::Left))
                {
                    spaces.at(i) = 0;
                    spaces.at(i+1) = 1;
                }
                else if (IsAlignSet(cAlign, Align::Right))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 0;
                }
                else if (IsAlignSet(cAlign, Align::Center) || IsAlignSet(cAlign, Align::HCenter))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 1;
                }
            }

            Size::value_type spacesToUse{0};
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    ++spacesToUse;

            Size::value_type spaceWidth{(spacesToUse != 0) ? static_cast<Size::value_type>(width) / spacesToUse : 0};
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    spaces.at(i) = spaceWidth;
        }

        return spaces;
    }

    Point::value_type HBox::alignChildV(size_type index, const Size& parentSize, const Size& childSize)
    {
        Point::value_type posy{0};
        const auto& child{ at(index) };

        // Total size with margin and padding.
        Size cSize{ at(index)->calcOuterFromSize(childSize) };

        // Align
        std::underlying_type<Align>::type cAlign = at(index)->getAlign();
        if (IsAlignSet(cAlign, Align::Bottom))
            posy = static_cast<Point::value_type>(parentSize.height - cSize.height);
        else if (IsAlignSet(cAlign, Align::Center) || IsAlignSet(cAlign, Align::VCenter))
            posy = static_cast<Point::value_type>((parentSize.height / 2) - (cSize.height / 2));

        // Offset position.
        // Maybe remove "AddWithoutOverflow" since it is highly unlikely that it overflows int32.
        posy = AddWithoutOverflow(posy, static_cast<Point::value_type>(child->getMargin().top));

        return posy;
    }

    Size HBox::calcMinSize() const
    {
        Size contMinSize{Size::Min};
        for (auto it{ cbegin() }; it != cend(); ++it)
        {
            const Size minSize{ (*it)->calcOuterFromSize((*it)->getMinSize()) };

            contMinSize.height = (minSize.height > contMinSize.height) ? minSize.height : contMinSize.height;
            contMinSize.width = AddWithoutOverflow(contMinSize.width, minSize.width);
        }

        contMinSize = calcOuterFromSize(contMinSize);

        return contMinSize;
    }

    Size HBox::calcMaxSize() const
    {
        Size contMaxSize{Size::Max};
        for (auto it{ cbegin() }; it != cend(); ++it)
        {
            const Size maxSize{ (*it)->calcOuterFromSize((*it)->getMaxSize()) };

            contMaxSize.height = (maxSize.height > contMaxSize.height) ? maxSize.height : contMaxSize.height;
            contMaxSize.width = AddWithoutOverflow(contMaxSize.width, maxSize.width);
        }

        contMaxSize = calcOuterFromSize(contMaxSize);

        return contMaxSize;
    }
}
