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
    
    const Margin Margin::Auto{pTK::Auto, pTK::Auto, pTK::Auto, pTK::Auto};
}
