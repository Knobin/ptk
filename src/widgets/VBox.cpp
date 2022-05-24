//
//  widgets/VBox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

// pTK Headers
#include "ptk/widgets/VBox.hpp"

// C++ Headers
#include <cmath>
#include <utility>

namespace pTK
{
    VBox::VBox()
        : WidgetContainer()
    {
        setMaxSize(Size::Max);
    }

    void VBox::onAdd(const Ref<Widget>&)
    {
        const Size minLayoutSize{calcMinSize()};
        setMinSize(minLayoutSize);
        const Size vbSize{getSize()};

        if ((minLayoutSize.width > vbSize.width) || (minLayoutSize.height > vbSize.height))
        {
            /** Children will not fit in the current size.

             Set minimal size to VBox and set minimal size to each child.
             */
            expandOnAdd(minLayoutSize);
        }else
        {
            /** Children will fit in the current size.

             Only need to resize and position children.
            */
            refitContent(vbSize);
        }
    }

    void VBox::onRemove(const Ref<Widget>&)
    {
        refitContent(getSize());
    }

    void VBox::onChildUpdate(size_type)
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

    void VBox::expandOnAdd(const Size& newSize)
    {
        Size layoutSize{newSize};
        const Size vbSize{getSize()};
        layoutSize.height   = (vbSize.height > layoutSize.height) ? vbSize.height : layoutSize.height;
        layoutSize.width    = (vbSize.width > layoutSize.width) ? vbSize.width : layoutSize.width;
        setSize(layoutSize); // this will generate a Resize event.

        const size_t children{container().size()};
        Point vbPos{getPosition()};
        constexpr int int_max = std::numeric_limits<int>::max();
        for (size_type i{0}; i < children; ++i)
        {
            const auto child{at(i)};
            const Size cSize{child->getMinSize()};

            /** We should not have to consider auto margin for children.
                This function should only be called when adding and children wont fit in current
                size and we only expand so the they will fit with their size and margin.
                Auto margin is not included.
             */
            Margin cMargin{child->getMargin()};
            cMargin.top = (cMargin.top > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.top;
            cMargin.bottom = (cMargin.bottom > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.bottom;

            vbPos.y += static_cast<Point::value_type>(cMargin.top);
            if (child->getSize() != cSize)
                child->setSize(cSize);
            child->setPosHint(Point(vbPos.x + alignChildH(i, newSize, cSize), vbPos.y));
            vbPos.y += static_cast<Point::value_type>(cSize.height) + static_cast<Point::value_type>(cMargin.bottom);
        }
    }

    void VBox::refitContent(const Size& nsize)
    {
        const size_type children{container().size()};
        if (children == 0)
            return;

        const Size vbSize{nsize};
        std::vector<Size> sizes(children);

        // Initialize sizes.
        for (uint i{0}; i < children; ++i)
        {
            Widget* widget = at(i).get();
            const Limits limits = widget->getLimitsWithSizePolicy();
            sizes.at(i) = widget->calcOuterFromSize(limits.min);

            const Size::value_type maxWidth{ widget->calcOuterFromSize(limits.max).width };
            sizes.at(i).width = (vbSize.width > maxWidth) ? maxWidth : vbSize.width;
        }

        // Expand children to its max sizes possible.
        const Size::value_type heightLeft = vbSize.height - calcMinSize().height;
        Size::value_type totalEachLeft = heightLeft;

        // Distribute heightLeft.
        // Need to fix this some time.
        // TODO: it takes many iteration before the height is distributed, especially if only 1 can grow.
        Size::value_type lastEachLeft = totalEachLeft;
        while (totalEachLeft > 0)
        {
            Size::value_type eachAdd{static_cast<Size::value_type>(std::floor(static_cast<float>(totalEachLeft) / static_cast<float>(children)))};
            eachAdd = (totalEachLeft < static_cast<Size::value_type>(children)) ? 1 : eachAdd;
            bool done{true};
            for (size_type i{0}; i < children; ++i)
            {
                Widget* widget = at(i).get();
                const Limits limits = widget->getLimitsWithSizePolicy();

                // Max - Min
                const Size::value_type maxHeight = widget->calcOuterFromSize(limits.max).height;
                const Size::value_type delta{maxHeight - sizes.at(i).height};

                if (delta > 0)
                {
                    if (eachAdd > delta)
                    {
                        sizes.at(i).height += delta;
                        totalEachLeft -= (delta);
                        done = false;
                    }else
                    {
                        sizes.at(i).height += eachAdd;
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
        const std::vector<Size::value_type> spaces{calcSpaces(totalEachLeft)};

        // Set sizes to childs and spaces.
        Point vbPos{getPosition()};
        constexpr int int_max = std::numeric_limits<int>::max();
        for (size_type i{0}; i != children; i++)
        {
            auto child{at(i)};
            Size cSize{sizes.at(i)};

            Margin cMargin{child->getMargin()};
            cMargin.top = (cMargin.top > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.top;
            cMargin.bottom = (cMargin.bottom > static_cast<Margin::value_type>(int_max)) ? int_max : cMargin.bottom;

            cSize.height -= static_cast<Size::value_type>(cMargin.top + cMargin.bottom);
            cSize.width -= static_cast<Size::value_type>(cMargin.left + cMargin.right);
            vbPos.y += static_cast<Point::value_type>(cMargin.top) + static_cast<Point::value_type>(spaces.at(i));
            if (child->getSize() != cSize)
                child->setSize(cSize);
            child->setPosHint(Point(vbPos.x + alignChildH(i, vbSize, cSize), vbPos.y));
            vbPos.y += static_cast<Point::value_type>(cSize.height) + static_cast<Point::value_type>(cMargin.bottom);
        }
    }

    void VBox::onSizeChange(const Size& size)
    {
        refitContent(size);
    }

    std::vector<Size::value_type> VBox::calcSpaces(Size::value_type height)
    {
        const size_type children{container().size()};
        const size_type spaceCount{children + 1};
        std::vector<Size::value_type> spaces(spaceCount);
        if (height != 0)
        {
            for (size_type i{0}; i != children; i++)
            {
                const std::underlying_type<Align>::type cAlign{at(i)->getAlign()};

                if (IsAlignSet(cAlign, Align::Top))
                {
                    spaces.at(i) = 0;
                    spaces.at(i+1) = 1;
                }
                else if (IsAlignSet(cAlign, Align::Bottom))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 0;
                }
                else if (IsAlignSet(cAlign, Align::Center) || IsAlignSet(cAlign, Align::VCenter))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 1;
                }
            }

            Size::value_type spacesToUse{0};
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    ++spacesToUse;

            Size::value_type spaceHeight{(spacesToUse != 0) ? height / spacesToUse : 0};
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    spaces.at(i) = spaceHeight;

        }

        return spaces;
    }

    Point::value_type VBox::alignChildH(size_type index, const Size& parentSize, const Size& childSize)
    {
        Point::value_type posx{0};
        const auto& child{ at(index) };

        // Total size including margin.
        Size cSize{ child->calcOuterFromSize(childSize) };

        // Align.
        const std::underlying_type<Align>::type cAlign{at(index)->getAlign()};
        if (IsAlignSet(cAlign, Align::Right))
            posx = static_cast<Point::value_type>(parentSize.width - cSize.width);
        else if (IsAlignSet(cAlign, Align::Center) || IsAlignSet(cAlign, Align::HCenter))
            posx = static_cast<Point::value_type>((parentSize.width/2) - (cSize.width/2));

        // Offset position.
        // Maybe remove "AddWithoutOverflow" since it is highly unlikely that it overflows int32.
        posx = AddWithoutOverflow(posx, static_cast<Point::value_type>(child->getMargin().left));

        return posx;
    }

    Size VBox::calcMinSize() const
    {
        Size contMinSize{Size::Min};
        for (auto it{cbegin()}; it != cend(); ++it)
        {
            const Limits limits{(*it)->getLimitsWithSizePolicy()};
            const Size minSize{(*it)->calcOuterFromSize(limits.min)};

            contMinSize.height = AddWithoutOverflow(contMinSize.height, minSize.height);
            contMinSize.width = (minSize.width > contMinSize.width) ? minSize.width : contMinSize.width;
        }

        return calcOuterFromSize(contMinSize);
    }

    Size VBox::calcMaxSize() const
    {
        Size contMaxSize{Size::Max};
        for (auto it{ cbegin() }; it != cend(); ++it)
        {
            const Limits limits{(*it)->getLimitsWithSizePolicy()};
            const Size maxSize{(*it)->calcOuterFromSize(limits.max)};

            contMaxSize.height = AddWithoutOverflow(contMaxSize.height, maxSize.height);
            contMaxSize.width = (maxSize.width > contMaxSize.width) ? maxSize.width : contMaxSize.width;
        }
        
        return calcOuterFromSize(contMaxSize);
    }
}
