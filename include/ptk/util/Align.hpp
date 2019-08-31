//
//  util/Layout.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-29s.
//

#ifndef PTK_UTIL_LAYOUT_HPPs
#define PTK_UTIL_LAYOUT_HPPs

// Local Headers
#include "ptk/core/Types.hpp"

namespace pTK
{
    namespace Align
    {
        struct Directions
        {
            int32 top;
            int32 bottom;
            int32 left;
            int32 right;
        };
    }
    
    struct Margin : public Align::Directions
    {
        Margin();
        Margin(int32 mTop, int32 mBottom, int32 mLeft, int32 mRight);
        ~Margin() = default;
        
        static const Margin Auto;
    };
    
    // TODO: Padding
    // using Padding = Directions;
}

#endif // PTK_UTIL_LAYOUT_HPPs
