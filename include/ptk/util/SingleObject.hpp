//
//  util/SingleObject.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-29.
//

#ifndef PTK_UTIL_SINGLEOBJECT_HPP
#define PTK_UTIL_SINGLEOBJECT_HPP

// pTK Headers
#include "ptk/util/NonCopyable.hpp"
#include "ptk/util/NonMovable.hpp"

namespace pTK
{
    /** SingleObject class implementation.

        Class for forbidding copying and moving.
    */
    class PTK_API SingleObject : public NonMovable, public NonCopyable
    {
    public:
        virtual ~SingleObject() = default;
    };
} // namespace pTK

#endif // PTK_UTIL_SINGLEOBJECT_HPP
