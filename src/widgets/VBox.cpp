//
//  widgets/VBox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

// Local Headers
#include "ptk/widgets/VBox.hpp"

// C++ Headers
#include <cmath>
#include <utility>

namespace pTK
{
    VBox::VBox()
        : Box()
    {

    }

    void VBox::onAdd(const Ref<Widget>&)
    {
        const Size minLayoutSize{calcMinSize()};
        setMinSize(minLayoutSize);
        const Size vbSize{getSize()};
        //setLimits(minLayoutSize, calculateMaxSize());

        if ((minLayoutSize.width > vbSize.width) || (minLayoutSize.height > vbSize.height))
        {
            /** Children will not fit in the current size.

             Set minimal size to VBox and set minimal size to each child.
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

    void VBox::onRemove(const Ref<Widget>&)
    {
        refitContent(getSize());
    }

    void VBox::onChildUpdate(size_type)
    {
        if (!isConstSize())
        {
            const Size cMaxSize{calcMaxSize()};
            Size maxSize{getMaxSize()};
            if (cMaxSize > maxSize)
                maxSize = cMaxSize;

            const Size cMinSize{calcMinSize()};
            Size minSize{getMinSize()};
            if (cMinSize > minSize)
                minSize = cMinSize;

            setLimits(minSize, maxSize);
        }
    }

    void VBox::expandOnAdd(const Size& newSize)
    {
        Size layoutSize{newSize};
        const Size vbSize{getSize()};
        layoutSize.height   = (vbSize.height > layoutSize.height) ? vbSize.height : layoutSize.height;
        layoutSize.width    = (vbSize.width > layoutSize.width) ? vbSize.width : layoutSize.width;
        setSize(layoutSize); // this will generate a Resize event.

        const size_t children{size()};
        Point vbPos{getPosition()};
        for (size_type i{0}; i < children; ++i)
        {
            const auto child{at(i)};
            const Size cSize{child->getMinSize()};

            /** We should not have to consider auto margin for children.
                This function should only be called when adding and children wont fit in current
                size and we only expand so the they will fit with their size and margin.
                Auto margin is not included.
             */
            const Margin cMargin{child->getMargin()};
            const Padding cPadding{child->getPadding()};
            vbPos.y += cMargin.top + cPadding.top;
            child->setSize(cSize);
            child->setPosHint(Point(vbPos.x + alignChildH(i, newSize, cSize), vbPos.y));
            vbPos.y += cSize.height + cMargin.bottom + cPadding.bottom;
        }
    }

    void VBox::refitContent(const Size& nsize)
    {
        const Size vbSize{nsize};
        const size_type children{size()};
        std::vector<Size> sizes(children);

        // Initialize sizes.
        for (uint i{0}; i < children; ++i)
        {
            sizes.at(i) = at(i)->getMinSize();
            const Size::value_type maxWidth{at(i)->getMaxSize().width};
            sizes.at(i).width = (vbSize.width > maxWidth) ? maxWidth : vbSize.width;
        }

        // Expand children to its max sizes possible.
        const Size::value_type heightLeft      = vbSize.height - calcMinSize().height;
        Size::value_type totalEachLeft   = heightLeft;

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
                const Size::value_type delta{at(i)->getMaxSize().height - sizes.at(i).height};

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

            // We cannot add more to the widgets.
            if ((done) || (eachAdd == 0))
                break;
        }

        // TODO: Fix if we break in while loop (size left unused).
        const std::vector<Size::value_type> spaces{calcSpaces(totalEachLeft)};

        // Set sizes to childs and spaces.
        Point vbPos{getPosition()};
        for (size_type i{0}; i != children; i++)
        {
            auto child{at(i)};
            const Size cSize{sizes.at(i)};
            const Margin cMargin{child->getMargin()};
            const Padding cPadding{child->getPadding()};
            vbPos.y += cMargin.top + cPadding.top + spaces.at(i);
            child->setSize(cSize);
            child->setPosHint(Point(vbPos.x + alignChildH(i, vbSize, cSize), vbPos.y));
            vbPos.y += cSize.height + cMargin.bottom + cPadding.bottom;
        }
    }

    void VBox::onSizeChange(const Size& size)
    {
        refitContent(size);
    }

    std::vector<Size::value_type> VBox::calcSpaces(Size::value_type height)
    {
        const size_type children{size()};
        const size_type spaceCount{size() + 1};
        std::vector<Size::value_type> spaces(spaceCount);
        if (height != 0)
        {
            for (size_type i{0}; i != children; i++)
            {
                const std::underlying_type<Align>::type cAlign{at(i)->getAlign()};

                if (isAlignSet(cAlign, Align::Top))
                {
                    spaces.at(i) = 0;
                    spaces.at(i+1) = 1;
                }
                else if (isAlignSet(cAlign, Align::Bottom))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 0;
                }
                else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::VCenter))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 1;
                }
            }

            uint spacesToUse{0};
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    ++spacesToUse;

            uint spaceHeight{(spacesToUse != 0) ? height / spacesToUse : 0};
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    spaces.at(i) = spaceHeight;

        }

        return spaces;
    }

    Point::value_type VBox::alignChildH(size_type index, const Size& parentSize, const Size& childSize)
    {
        Point::value_type posx{0};
        Size cSize{childSize};
        const Margin cMargin{at(index)->getMargin()};
        const Padding cPadding{at(index)->getPadding()};

        // Pre
        cSize.width += static_cast<Size::value_type>(cPadding.left + cPadding.right);

        // Align
        const std::underlying_type<Align>::type cAlign{at(index)->getAlign()};
        if (isAlignSet(cAlign, Align::Right))
            posx = static_cast<Point::value_type>(parentSize.width - cSize.width);
        else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::HCenter))
            posx = static_cast<Point::value_type>((parentSize.width/2) - (cSize.width/2));

        // Post
        posx += static_cast<Point::value_type>(cMargin.left);
        posx += static_cast<Point::value_type>(cPadding.left);

        return posx;
    }

    Size VBox::calcMinSize() const
    {
        Size contentMinSize{Size::Min};
        for (auto it{cbegin()}; it != cend(); ++it)
        {
            const Padding cPadding{(*it)->getPadding()};
            const Padding::value_type vPadding{cPadding.top + cPadding.bottom};
            const Padding::value_type hPadding{cPadding.left + cPadding.right};

            const Size cMinSize{(*it)->getMinSize()};
            contentMinSize.height += static_cast<Size::value_type>(vPadding + cMinSize.height);
            contentMinSize.width =
                ((cMinSize.width + static_cast<Size::value_type>(hPadding)) > contentMinSize.width)
                ? cMinSize.width + static_cast<Size::value_type>(hPadding) : contentMinSize.width;
        }

        return contentMinSize;
    }

    Size VBox::calcMaxSize() const
    {
        Size contentMaxSize{ Size::Max };
        for (auto it{ cbegin() }; it != cend(); ++it)
        {
            const Padding cPadding{(*it)->getPadding()};
            const Padding::value_type vPadding{cPadding.top + cPadding.bottom};
            const Padding::value_type hPadding{cPadding.left + cPadding.right};

            const Size maxSize{(*it)->getMaxSize()};
            contentMaxSize.height += (maxSize.height < (contentMaxSize.height - static_cast<Size::value_type>(vPadding)))
                ? maxSize.height + static_cast<Size::value_type>(vPadding) : contentMaxSize.height;
            contentMaxSize.width += (maxSize.width < (contentMaxSize.width - static_cast<Size::value_type>(hPadding)))
                ? maxSize.width + static_cast<Size::value_type>(hPadding) : contentMaxSize.width;
        }

        return contentMaxSize;
    }
}
