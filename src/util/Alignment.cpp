//
//  util/Align.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-29s.
//

// Local Headers
#include "ptk/util/Alignment.hpp"

namespace pTK
{
    bool isAlignSet(std::underlying_type<Align>::type number, Align align)
    {
        return ((number & align) == align);
    }
    
    Margin::Margin()
        : Directions{0, 0, 0, 0}
    {
    }

    Margin::Margin(Directions directions)
        : Directions{directions}
    {
    }
    
    Margin::Margin(value_type mTop, value_type mBottom, value_type mLeft, value_type mRight)
        : Directions{mTop, mBottom, mLeft, mRight}
    {
    }
}
