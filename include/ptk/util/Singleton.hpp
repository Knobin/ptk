//
//  util/Singleton.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-29.
//

#ifndef PTK_UTIL_SINGLETON_HPP
#define PTK_UTIL_SINGLETON_HPP

// pTK Headers
#include "ptk/util/NonCopyable.hpp"
#include "ptk/util/NonMovable.hpp"

namespace pTK
{
    /** Singeton class implementation.
     
        Class for forbidding copying and moving.
    */
    class Singleton : public NonMovable, public NonCopyable
    {
    public:
        virtual ~Singleton() = default;
    };
}

#endif // PTK_UTIL_SINGLETON_HPP

