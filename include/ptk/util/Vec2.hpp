//
//  util/Vec2.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_UTIL_VEC2_HPP
#define PTK_UTIL_VEC2_HPP

// Local Headers
#include "ptk/core/Types.hpp"

namespace pTK
{
    /** Vec2 class implementation.
         
     This is a basic template class for holding two 
     coordinates (or values) x and y.
     */
    template <typename T>
    class Vec2
    {
    public:
        /** Constructs Vec2 with default values.
         
         @return    default initialized Vec2
         */
        Vec2()
            : x{}, y{}
        {
        }
        
        /** Constructs Vec2 with default values with x and y.
         
         @return    default initialized Vec2
         */
        Vec2(T t_x, T t_y)
            : x{t_x}, y{t_y}
        {
        }
        
        /** Constructs Vec2 with default values from another Vec2. (Copy)
         
         This constructor can copy ant templated Vec2, as long as static_cast
         can do it. If not, you will get a compile error.
         
         @param vec     Vec2 to copy from
         @return        default initialized Vec2
         */
        template <typename U>
        explicit Vec2(const Vec2<U>& vec)
            : x{static_cast<T>(vec.x)}, y{static_cast<T>(vec.y)}
        {
        }
        
        /** Constructs Vec2 with default values from another Vec2. (Copy)
         
         This constructor can copy ant templated Vec2, as long as static_cast
         can do it. If not, you will get a compile error.
         
         @param vec     Vec2 to copy from
         @return        default initialized Vec2
         */
        template <typename U>
        Vec2<T>& operator=( const Vec2<U>& vec )
        {
            x = static_cast<T>(vec.x);
            y = static_cast<T>(vec.y);
            
            return *this;
        }
        
        // Variables.
        T x;
        T y;
    };
    
    // Comparison operators.
    template <typename T>
    bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return ( (lhs.x == rhs.x) && (lhs.y == rhs.y) );
    }
    
    template <typename T>
    bool operator!=(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return !(lhs == rhs);
    }
    
    // Binary arithmetic operators.
    template <typename T>
    Vec2<T> operator+(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        Vec2<T> new_Vec2;
        new_Vec2.x = lhs.x + rhs.x;
        new_Vec2.y = lhs.y + rhs.y;
        
        return new_Vec2;
    }
    
    template <typename T>
    Vec2<T> operator-(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        Vec2<T> new_Vec2;
        new_Vec2.x = lhs.x - rhs.x;
        new_Vec2.y = lhs.y - rhs.y;
        
        return new_Vec2;
    }
    
    template <typename T>
    Vec2<T> operator*(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        Vec2<T> new_Vec2;
        new_Vec2.x = lhs.x * rhs.x;
        new_Vec2.y = lhs.y * rhs.y;
        
        return new_Vec2;
    }
    
    template <typename T>
    Vec2<T> operator/(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        Vec2<T> new_Vec2;
        new_Vec2.x = lhs.x / rhs.x;
        new_Vec2.y = lhs.y / rhs.y;
        
        return new_Vec2;
    }
    
    template <typename T>
    Vec2<T>& operator+=(Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return lhs = lhs + rhs;
    }
    
    template <typename T>
    Vec2<T>& operator-=(Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return lhs = lhs - rhs;
    }
    
    template <typename T>
    Vec2<T>& operator*=(Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return lhs = lhs * rhs;
    }
    
    template <typename T>
    Vec2<T>& operator/=(Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return lhs = lhs / rhs;
    }
	
    using Vec2f = Vec2<float>;
    using Vec2u = Vec2<uint>;
    using Vec2i = Vec2<int32>;
}

#endif // PTK_UTIL_VEC2_HPP
