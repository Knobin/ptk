//
//  util/Layout.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-29s.
//

#ifndef PTK_UTIL_ALIGNMENT_HPP
#define PTK_UTIL_ALIGNMENT_HPP

// Local Headers
#include "ptk/core/Types.hpp"

// C++ Headers
#include <type_traits>

namespace pTK
{
    enum Align : int32
    {
        Top     = 0x0001,
        Bottom  = 0x0002,
        Left    = 0x0004,
        Right   = 0x0008,

        HCenter = 0x0020,
        VCenter = 0x0040,
        Center  = 0x0080
    };
    
    bool isAlignSet(std::underlying_type<Align>::type number, Align align);

    /** Directions struct implementation.
     
     Pretty self-explanatory.
     */
    struct Directions
    {
        using value_type = int32;

        value_type top;
        value_type bottom;
        value_type left;
        value_type right;
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
        Margin(value_type mTop, value_type mBottom, value_type mLeft, value_type mRight);
        
        ~Margin() = default;
    };

    // TODO: Padding
    // using Padding = Directions;
}

#endif // PTK_UTIL_ALIGNMENT_HPP
