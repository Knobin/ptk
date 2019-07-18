//
//  util/Position.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

// Local Headers
#include "ptk/util/Position.hpp"

namespace pTK
{
    Position::Position()
        : x{0}, y{0}
    {
        
    }
    
    Position::Position(int t_x, int t_y)
        : x{t_x}, y{t_y}
    {
        
    }

    void Position::setPosition(int t_x, int t_y)
    {
        x = t_x;
        y = t_y;
    }
    
    // Comparison operators.
    bool operator==(const Position& lhs, const Position& rhs)
    {
        return ((lhs.x == rhs.x) && (lhs.y == rhs.y));
    }
    
    bool operator!=(const Position& lhs, const Position& rhs)
    {
        return !(lhs == rhs);
    }
    
    // Binary arithmetic operators.
    Position operator+(const Position& lhs, const Position& rhs)
    {
        Position newPosition;
        newPosition.x = lhs.x + rhs.x;
        newPosition.y = lhs.y + rhs.y;
        
        return newPosition;
    }
    
    Position operator-(const Position& lhs, const Position& rhs)
    {
        Position newPosition;
        newPosition.x = lhs.x - rhs.x;
        newPosition.y = lhs.y - rhs.y;
        
        return newPosition;
    }
    
    Position operator*(const Position& lhs, const Position& rhs)
    {
        Position newPosition;
        newPosition.x = lhs.x * rhs.x;
        newPosition.y = lhs.y * rhs.y;
        
        return newPosition;
    }
    
    Position operator/(const Position& lhs, const Position& rhs)
    {
        Position newPosition;
        newPosition.x = lhs.x / rhs.x;
        newPosition.y = lhs.y / rhs.y;
        
        return newPosition;
    }
    
    Position& operator+=(Position& lhs, const Position& rhs)
    {
        return lhs = lhs + rhs;
    }
    
    Position& operator-=(Position& lhs, const Position& rhs)
    {
        return lhs = lhs - rhs;
    }
    
    Position& operator*=(Position& lhs, const Position& rhs)
    {
        return lhs = lhs * rhs;
    }
    
    Position& operator/=(Position& lhs, const Position& rhs)
    {
        return lhs = lhs / rhs;
    }
}
