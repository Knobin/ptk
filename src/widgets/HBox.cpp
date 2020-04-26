//
//  widgets/HBox.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-11-27.
//

// Local Headers
#include "ptk/widgets/HBox.hpp"

namespace pTK
{       
    HBox::HBox()
        : Box()
    {

    }
    
    void HBox::onAdd(const Ref<Widget>&)
    {
        Size vbSize{getSize()};
        Size minLayoutSize{calcMinSize()};
        setMinSize(minLayoutSize);
        
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
        refitContent(getSize());
    }
    
    void HBox::expandOnAdd(const Size& newSize)
    {
        Size layoutSize{newSize};
        const Size vbSize{getSize()};
        Point vbPos{getPosition()};
        
        layoutSize.height   = (vbSize.height > layoutSize.height) ? vbSize.height : layoutSize.height;
        layoutSize.width    = (vbSize.width > layoutSize.width) ? vbSize.width : layoutSize.width;
        
        setSize(layoutSize); // this will generate a Resize event.
        const size_type children{size()};
        for (size_type i{0}; i < children; ++i)
        {
            auto child = at(i);
            Size cSize = child->getMinSize();
            
            /** We should not have to consider auto margin for children.
                This function should only be called when adding and children wont fit in current
                size and we only expand so the they will fit with their size and margin.
                Auto margin is not included.
             */
            child->setSize(cSize);
            Margin cMargin{child->getMargin()};
            Padding cPadding{child->getPadding()};
            vbPos.x += cMargin.left + cPadding.left;;
            child->setPosHint(Point(vbPos.x, vbPos.y + alignChildV(i, vbSize, cSize)));
            vbPos.x += cSize.width + cMargin.right + cPadding.right;
        }
    }
    
    void HBox::refitContent(const Size& nsize)
    {
        const Size vbSize{nsize};
        const size_type children{size()};
        std::vector<Size> sizes(children);
        
        // Initialize sizes.
        for (size_type i{0}; i < children; ++i)
        {
            sizes.at(i) = at(i)->getMinSize();

            Size::value_type maxHeight = at(i)->getMaxSize().height;
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
                const Size::value_type delta{at(i)->getMaxSize().width - sizes.at(i).width};
                
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
        for (size_type i{0}; i != children; i++)
        {
            auto child{at(i)};
            const Size cSize{sizes.at(i)};
            const Margin cMargin{child->getMargin()};
            const Padding cPadding{child->getPadding()};
            vbPos.x += cMargin.left + cPadding.left + spaces.at(i);
            child->setSize(cSize);
            child->setPosHint(Point(vbPos.x, vbPos.y + alignChildV(i, vbSize, cSize)));
            vbPos.x += cSize.width + cMargin.right + cPadding.right;
        }
    }

    void HBox::onResize(const Size& size)
    {
        refitContent(size);
    }
    
    std::vector<Size::value_type> HBox::calcSpaces(Size::value_type width)
    {
        const size_type children{size()};
        const size_type spaceCount{size() + 1};
        std::vector<Size::value_type> spaces(spaceCount);
        if (width != 0)
        {
            for (size_type i{0}; i != children; i++)
            {
                const std::underlying_type<Align>::type cAlign{at(i)->getAlign()};
                
                if (isAlignSet(cAlign, Align::Left))
                {
                    spaces.at(i) = 0;
                    spaces.at(i+1) = 1;
                }
                else if (isAlignSet(cAlign, Align::Right))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 0;
                }
                else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::HCenter))
                {
                    spaces.at(i) = 1;
                    spaces.at(i+1) = 1;
                }
            }
            
            uint spacesToUse{0};
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    ++spacesToUse;
            
            uint spaceWidth{(spacesToUse != 0) ? static_cast<uint>(width) / spacesToUse : 0};
            for (size_type i{0}; i != spaceCount; i++)
                if (spaces.at(i) == 1)
                    spaces.at(i) = spaceWidth;
        }
        
        return spaces;
    }
    
    Point::value_type HBox::alignChildV(size_type index, const Size& parentSize, const Size& childSize)
    {
        Point::value_type posy{0};
        Size cSize{childSize};
        const auto child{at(index)};
        const Margin cMargin{child->getMargin()};
        const Padding cPadding{child->getPadding()};

        // Pre
        cSize.height += static_cast<Size::value_type>(cPadding.top + cPadding.bottom);

        // Align
        std::underlying_type<Align>::type cAlign = at(index)->getAlign();
        if (isAlignSet(cAlign, Align::Bottom))
            posy = static_cast<Point::value_type>(parentSize.height - cSize.height);
        else if (isAlignSet(cAlign, Align::Center) || isAlignSet(cAlign, Align::VCenter))
            posy = static_cast<Point::value_type>((parentSize.height / 2) - (cSize.height / 2));

        // Post
        posy += static_cast<Point::value_type>(cMargin.top);
        posy += static_cast<Point::value_type>(cPadding.top);

        return posy;
    }

    Size HBox::calcMinSize() const
    {
        Size contentMinSize{Size::Min};
        for (auto it{ cbegin() }; it != cend(); ++it)
        {
            const Padding cPadding{(*it)->getPadding()};
            const Padding::value_type vPadding{cPadding.top + cPadding.bottom};
            const Padding::value_type hPadding{cPadding.left + cPadding.right};

            const Size cMinSize{(*it)->getMinSize()};
            contentMinSize.width += static_cast<Size::value_type>(hPadding + cMinSize.width);
            contentMinSize.height =
                ((cMinSize.height + static_cast<Size::value_type>(vPadding)) > contentMinSize.height)
                ? cMinSize.height + static_cast<Size::value_type>(vPadding) : contentMinSize.height;
        }

        return contentMinSize;
    }

    Size HBox::calcMaxSize() const
    {
        Size contentMaxSize{Size::Max};
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

