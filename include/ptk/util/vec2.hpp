//
//  util/vec2.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_UTIL_VEC2_HPP
#define PTK_UTIL_VEC2_HPP

namespace pTK
{
    template <typename T>
    class Vec2
    {
    public:
        Vec2()
            : x{0}, y{0}
        {
        }
        
        Vec2(T t_x, T t_y)
            : x{t_x}, y{t_y}
        {
        }
        
        template <typename S>
        explicit Vec2(const Vec2<S>& vec)
            : x{static_cast<T>(vec.x)}, y{static_cast<T>(vec.y)}
        {
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
}

#endif // PTK_UTIL_VEC2_HPP
