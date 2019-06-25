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
        : m_x{0.0f}, m_y{0.0f}
    {
        
    }
    
    Position::Position(float x, float y)
        : m_x{x}, m_y{y}
    {
        
    }

    void Position::setPosition(float x, float y)
    {
        m_x = x;
        m_y = y;
    }
    
    void Position::setPosition(const Position& position)
    {
        m_x = position.getX();
        m_y = position.getY();
    }
    
    void Position::setX(float x)
    {
        m_x = x;
    }
    
    void Position::setY(float y)
    {
        m_y = y;
    }
    
    float Position::getX() const
    {
        return m_x;
    }
    
    float Position::getY() const
    {
        return m_y;
    }
    
    // Comparison operators.
    bool operator==(const Position& lhs, const Position& rhs)
    {
        return ((lhs.getX() == rhs.getX()) && (lhs.getY() == rhs.getY()));
    }
    
    bool operator!=(const Position& lhs, const Position& rhs)
    {
        return !(lhs == rhs);
    }
    
    // Binary arithmetic operators.
    Position operator+(const Position& lhs, const Position& rhs)
    {
        Position newPosition;
        newPosition.setX(lhs.getX() + rhs.getX());
        newPosition.setY(lhs.getY() + rhs.getY());
        
        return newPosition;
    }
    
    Position operator-(const Position& lhs, const Position& rhs)
    {
        Position newPosition;
        newPosition.setX(lhs.getX() - rhs.getX());
        newPosition.setY(lhs.getY() - rhs.getY());
        
        return newPosition;
    }
    
    Position operator*(const Position& lhs, const Position& rhs)
    {
        Position newPosition;
        newPosition.setX(lhs.getX() * rhs.getX());
        newPosition.setY(lhs.getY() * rhs.getY());
        
        return newPosition;
    }
    
    Position operator/(const Position& lhs, const Position& rhs)
    {
        Position newPosition;
        newPosition.setX(lhs.getX() / rhs.getX());
        newPosition.setY(lhs.getY() / rhs.getY());
        
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
