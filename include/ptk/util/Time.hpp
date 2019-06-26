//
//  util/Time.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-26.
//

#ifndef PTK_UTIL_TIME_HPP
#define PTK_UTIL_TIME_HPP

// C++ Headers
#include <cstdint>
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
        uint32_t nanoseconds() const;
        
        /** Function for retrieving the time passed in microseconds,
         since reset was called or object was created.
         
         @return    Time passed in microseconds
         */
        uint32_t microseconds() const;
        
        /** Function for retrieving the time passed in milliseconds,
         since reset was called or object was created.
         
         @return    Time passed in milliseconds
         */
        uint32_t milliseconds() const;
        
        /** Function for retrieving the time passed in seconds,
         since reset was called or object was created.
         
         @return    Time passed in seconds
         */
        uint32_t seconds() const;
        
        /** Function for retrieving the time passed in minutes,
         since reset was called or object was created.
         
         @return    Time passed in minutes
         */
        uint32_t minutes() const;
        
        /** Function for retrieving the time passed in hours,
         since reset was called or object was created.
         
         @return    Time passed in hours
         */
        uint32_t hours() const;
        
    private:
        std::chrono::high_resolution_clock::time_point m_start;
    };
}

#endif // PTK_UTIL_TIME_HPP
