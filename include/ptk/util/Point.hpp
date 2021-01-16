//
//  util/Point.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

#ifndef PTK_UTIL_POINT_HPP
#define PTK_UTIL_POINT_HPP

// pTK Headers
#include "ptk/core/Types.hpp"
#include "ptk/util/Vec2.hpp"

namespace pTK
{
    /** Point class implementation.
     
        This class is low level class handling position.
    */
    class Point
    {
    public:
        using value_type = int32;

    public:
        /** Constructs Point with default values.
         
            @return    default initialized Point
        */
        Point();

        /** Constructs Point with default values with v.

            @param v   Vec2 to copy from
            @return    default initialized Point
        */
        template<typename T>
        Point(const Vec2<T>& v);
        
        /** Constructs Point with default values with other_x and other_y.
         
            @param other_x     x position
            @param other_y     y position
            @return            default initialized Point
        */
        Point(value_type other_x, value_type other_y);
        
        /** Function for setting both x and y position.
         
            @param other_x   new x position
            @param other_y   new y position
        */
        void set(value_type other_x, value_type other_y);

        /** Assigns Point with default values with v.

            @param v   Vec2 to copy from
            @return    point reference
        */
        template <typename T>
        Point& operator=(const Vec2<T>& v);
        
        value_type x;
        value_type y;
    };
    
    // Comparison operators.
    bool operator==(const Point& lhs, const Point& rhs);
    bool operator!=(const Point& lhs, const Point& rhs);
    
    // Binary arithmetic operators.
    Point operator+(const Point& lhs, const Point& rhs);
    Point operator-(const Point& lhs, const Point& rhs);
    Point operator*(const Point& lhs, const Point& rhs);
    Point operator/(const Point& lhs, const Point& rhs);
    Point& operator+=(Point& lhs, const Point& rhs);
    Point& operator-=(Point& lhs, const Point& rhs);
    Point& operator*=(Point& lhs, const Point& rhs);
    Point& operator/=(Point& lhs, const Point& rhs);

    template<typename T>
    Point::Point(const Vec2<T>& v)
        : x{static_cast<Point::value_type>(v.x)},
            y{static_cast<Point::value_type>(v.y)}
    {

    }

    template <typename T>
    Point& Point::operator=(const Vec2<T>& v)
    {
        x = static_cast<Point::value_type>(v.x);
        y = static_cast<Point::value_type>(v.y);

        return *this;
    }
}

#endif // PTK_UTIL_POSITION_HPP
