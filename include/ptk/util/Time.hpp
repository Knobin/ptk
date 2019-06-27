//
//  util/Time.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-26.
//

#ifndef PTK_UTIL_TIME_HPP
#define PTK_UTIL_TIME_HPP

// Local Headers
#include "ptk/core/Types.hpp"

// C++ Headers
#include <chrono>

namespace pTK
{
    class Time
    {
    public:
        /** Constructs Time with default values.
         
         @return    default initialized Time
         */
        Time();
        
        /** Function for resetting the internal clock.
         
         */
        void reset();
        
        /** Function for retrieving the time passed in nanoseconds,
         since reset was called or object was created.
         
         @return    Time passed in nanoseconds
         */
        uint64 nanoseconds() const;
        
        /** Function for retrieving the time passed in microseconds,
         since reset was called or object was created.
         
         @return    Time passed in microseconds
         */
        uint64 microseconds() const;
        
        /** Function for retrieving the time passed in milliseconds,
         since reset was called or object was created.
         
         @return    Time passed in milliseconds
         */
        uint64 milliseconds() const;
        
        /** Function for retrieving the time passed in seconds,
         since reset was called or object was created.
         
         @return    Time passed in seconds
         */
        uint64 seconds() const;
        
        /** Function for retrieving the time passed in minutes,
         since reset was called or object was created.
         
         @return    Time passed in minutes
         */
        uint32 minutes() const;
        
        /** Function for retrieving the time passed in hours,
         since reset was called or object was created.
         
         @return    Time passed in hours
         */
        uint32 hours() const;
        
    private:
        std::chrono::high_resolution_clock::time_point m_start;
    };
}

#endif // PTK_UTIL_TIME_HPP
