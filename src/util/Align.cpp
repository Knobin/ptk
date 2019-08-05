//
//  util/Align.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-29s.
//

// Local Headers
#include "ptk/util/Align.hpp"

namespace pTK
{
    Margin::Margin()
        : Directions{0, 0, 0, 0}
    {
    }
    
    Margin::Margin(int32 mTop, int32 mBottom, int32 mLeft, int32 mRight)
        : Directions{mTop, mBottom, mLeft, mRight}
    {
    }
    
    const int32 Margin::Style::Auto{-1};
    const Margin Margin::Auto{Style::Auto, Style::Auto, Style::Auto, Style::Auto};
}
