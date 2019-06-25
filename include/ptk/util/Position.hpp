//
//  util/Position.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

#ifndef PTK_UTIL_POSITION_HPP
#define PTK_UTIL_POSITION_HPP

namespace pTK
{
    class Position
    {
    public:
        /** Position class implementation.
         
         This class is low level class handling Position.
         */
        Position();
        Position(float x, float y);
        ~Position() = default;
        
        /** Function for setting Position.
         
         @param width   new width
         @param height  new height
         */
        void setPosition(float x, float y);
        
        /** Function for setting Position.
         
         @param Position    new width and height
         */
        void setPosition(const Position& position);
        
        /** Function for setting x position.
         
         @param width    new width
         */
        void setX(float x);
        
        /** Function for setting y position.
         
         @param height    new height
         */
        void setY(float y);
        
        float getX() const;
    
        float getY() const;
        
    private:
        float m_x;
        float m_y;
    };
    
    // Comparison operators.
    bool operator==(const Position& lhs, const Position& rhs);
    bool operator!=(const Position& lhs, const Position& rhs);
    
    // Binary arithmetic operators.
    Position operator+(const Position& lhs, const Position& rhs);
    Position operator-(const Position& lhs, const Position& rhs);
    Position operator*(const Position& lhs, const Position& rhs);
    Position operator/(const Position& lhs, const Position& rhs);
    Position& operator+=(Position& lhs, const Position& rhs);
    Position& operator-=(Position& lhs, const Position& rhs);
    Position& operator*=(Position& lhs, const Position& rhs);
    Position& operator/=(Position& lhs, const Position& rhs);
}



#endif // PTK_UTIL_POSITION_HPP
