//
//  util/Point.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

// Local Headers
#include "ptk/util/Point.hpp"

namespace pTK
{
    Point::Point()
        : x{0}, y{0}
    {
        
    }
    
    Point::Point(int t_x, int t_y)
        : x{t_x}, y{t_y}
    {
        
    }

    void Point::set(int t_x, int t_y)
    {
        x = t_x;
        y = t_y;
    }
    
    // Comparison operators.
    bool operator==(const Point& lhs, const Point& rhs)
    {
        return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
    }
    
    bool operator!=(const Point& lhs, const Point& rhs)
    {
        return !(lhs == rhs);
    }
    
    // Binary arithmetic operators.
    Point operator+(const Point& lhs, const Point& rhs)
    {
        Point newPoint;
        newPoint.x = lhs.x + rhs.x;
        newPoint.y = lhs.y + rhs.y;
        
        return newPoint;
    }
    
    Point operator-(const Point& lhs, const Point& rhs)
    {
        Point newPoint;
        newPoint.x = lhs.x - rhs.x;
        newPoint.y = lhs.y - rhs.y;
        
        return newPoint;
    }
    
    Point operator*(const Point& lhs, const Point& rhs)
    {
        Point newPoint;
        newPoint.x = lhs.x * rhs.x;
        newPoint.y = lhs.y * rhs.y;
        
        return newPoint;
    }
    
    Point operator/(const Point& lhs, const Point& rhs)
    {
        Point newPoint;
        newPoint.x = lhs.x / rhs.x;
        newPoint.y = lhs.y / rhs.y;
        
        return newPoint;
    }
    
    Point& operator+=(Point& lhs, const Point& rhs)
    {
        return lhs = lhs + rhs;
    }
    
    Point& operator-=(Point& lhs, const Point& rhs)
    {
        return lhs = lhs - rhs;
    }
    
    Point& operator*=(Point& lhs, const Point& rhs)
    {
        return lhs = lhs * rhs;
    }
    
    Point& operator/=(Point& lhs, const Point& rhs)
    {
        return lhs = lhs / rhs;
    }
}
