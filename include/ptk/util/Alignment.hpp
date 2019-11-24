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
    enum class Align
    {
        Top     = 0x0001,
        Bottom  = 0x0002,
        Left    = 0x0004,
        Right   = 0x0008,

        HCenter = 0x0020,
        VCenter = 0x0040,
        Center  = 0x0080
    };

    /** Directions struct implementation.
     
     Pretty self-explanatory.
     */
    struct Directions
    {
        int32 top;
        int32 bottom;
        int32 left;
        int32 right;
    };
    
    /** Margin class implementation.
     
     This class is for handling margin of all directions and
     keep track if directions have margin set to Auto as well.
     The Auto implementation for now is pretty bad and will, at
     some point, change. For now it works.
     
    */
    class Margin : public Directions
    {
    public:
        /** Constructs Margin with default values.
         
         @return    default initialized Margin
         */
        Margin();

        /** Constructs Margin with default values with directions.
         
         @param directions  margin for all directions
         @return            default initialized Margin
         */
        Margin(Directions directions);

        /** Constructs Margin with default values with directions.
         
         @param mTop    top margin
         @param mBottom bottom margin
         @param mLeft   left margin
         @param mRight  right margin
         @return        default initialized Margin
         */
        Margin(int32 mTop, int32 mBottom, int32 mLeft, int32 mRight);
        
        ~Margin() = default;
    };

    // TODO: Padding
    // using Padding = Directions;
}

#endif // PTK_UTIL_LAYOUT_HPPs
