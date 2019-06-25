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
        Size(float width, float height);
        ~Size() = default;
        
        /** Function for setting size.
         
         @param width   new width
         @param height  new height
         */
        void setSize(float width, float height);
        
        /** Function for setting size.
         
         @param size    new width and height
         */
        void setSize(const Size& size);
        
        /** Function for setting width.
         
         @param width    new width
         */
        void setWidth(float width);
        
        /** Function for setting height.
         
         @param height    new height
         */
        void setHeight(float height);
        
        float getHeight() const;
        
        float getWidth() const;
        
    private:
        float m_width;
        float m_height;
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
