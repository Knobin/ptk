//
//  util/Point.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

#ifndef PTK_UTIL_POINT_HPP
#define PTK_UTIL_POINT_HPP

namespace pTK
{
    /** Point class implementation.
     
     This class is low level class handling position.
     */
    class Point
    {
    public:
        /** Constructs Point with default values.
         
         @return    default initialized Point
         */
        Point();
        
        /** Constructs Point with default values with t_x and t_y.
         
         @return    default initialized Point
         */
        Point(int t_x, int t_y);
        
        ~Point() = default;
        
        /** Function for setting both x and y position.
         
         @param t_x     new x position
         @param t_y     new y position
         */
        void set(int t_x, int t_y);
        
        int x;
        int y;
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
}



#endif // PTK_UTIL_POSITION_HPP
