//
//  util/Point.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

// pTK Headers
#include "ptk/util/Point.hpp"

namespace pTK
{
    void Point::set(value_type other_x, value_type other_y)
    {
        x = other_x;
        y = other_y;
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
