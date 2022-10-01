//
//  util/NonMovable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-29.
//

#ifndef PTK_UTIL_NONMOVABLE_HPP
#define PTK_UTIL_NONMOVABLE_HPP

// pTK Headers
#include "ptk/core/Defines.hpp"

namespace pTK
{
    /** NonMovable class implementation.
     
        Class for forbidding moving.
    */
    class PTK_API NonMovable
    {
    public:
        /** Constructs NonMovable with default values.

        */
        NonMovable() = default;

        /** Destructor for NonMovable.

        */
        virtual ~NonMovable() = default;

        /** Deleted Move Constructor.

        */
        NonMovable(const NonMovable&) = delete;

        /** Deleted Move Assignment operator.

        */
        NonMovable& operator=(const NonMovable&) = delete;
    };
}

#endif // PTK_UTIL_NONMOVABLE_HPP

