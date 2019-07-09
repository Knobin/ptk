//
//  widgets/VBox.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-09.
//

#ifndef PTK_WIDGETS_VBOX_HPP
#define PTK_WIDGETS_VBOX_HPP

// Local Headers
#include "ptk/core/Box.hpp"

namespace pTK
{
    class VBox : public Box
    {
    public:
        VBox();
        virtual ~VBox() = default;
        
        bool add(const std::shared_ptr<Widget>& widget) override;
        // TODO: override remove
        
        bool drawChild(Widget* widget) override;
        
        void setPosHint(const Position& pos) override;
    };
}

#endif // PTK_WIDGETS_VBOX_HPP
