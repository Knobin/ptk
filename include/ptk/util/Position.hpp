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
    /** Position class implementation.
     
     This class is low level class handling position.
     */
    class Position
    {
    public:
        /** Constructs Position with default values.
         
         @return    default initialized Position
         */
        Position();
        
        /** Constructs Position with default values with t_x and t_y.
         
         @return    default initialized Position
         */
        Position(float t_x, float t_y);
        
        ~Position() = default;
        
        /** Function for setting both x and y position.
         
         @param t_x     new x position
         @param t_y     new y position
         */
        void setPosition(float t_x, float t_y);
        
        float x;
        float y;
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
