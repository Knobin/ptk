//
//  util/Size.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-24.
//

#ifndef PTK_UTIL_SIZE_HPP
#define PTK_UTIL_SIZE_HPP

namespace pTK
{
    class Size
    {
    public:
        /** Size class implementation.
         
         This class is low level class handling size.
         */
        Size();
        Size(int t_width, int t_height);
        ~Size() = default;
        
        /** Function for setting size.
         
         @param width   new width
         @param height  new height
         */
        void setSize(int t_width, int t_height);
        
        int width;
        int height;
    };
    
    // Comparison operators.
    bool operator==(const Size& lhs, const Size& rhs);
    bool operator!=(const Size& lhs, const Size& rhs);
    
    // Binary arithmetic operators.
    Size operator+(const Size& lhs, const Size& rhs);
    Size operator-(const Size& lhs, const Size& rhs);
    Size operator*(const Size& lhs, const Size& rhs);
    Size operator/(const Size& lhs, const Size& rhs);
    Size& operator+=(Size& lhs, const Size& rhs);
    Size& operator-=(Size& lhs, const Size& rhs);
    Size& operator*=(Size& lhs, const Size& rhs);
    Size& operator/=(Size& lhs, const Size& rhs);
}



#endif // PTK_UTIL_SIZE_HPP
