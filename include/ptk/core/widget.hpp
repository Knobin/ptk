//
//  core/widget.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_WIDGET_HPP
#define PTK_CORE_WIDGET_HPP

// Local Headers
#include "ptk/core/drawable.hpp"
#include "ptk/util/vec2.hpp"

// C++ Headers
#include <cstdint>

namespace pTK
{
    /** Widget class implementation.
     
     This class is low level class for widget, that
     has the essential component for rendering.
     */
    class Widget : public Drawable
    {
    public:
        /** Constructs Event with default values.
         
         @return    default initialized Widget
         */
        Widget();
        virtual ~Widget() = default;
        
        /** Function for retrieving the size.
         
         @return    Current size
         */
        const Vec2<float>& get_size() const;
        
        /** Function for setting size.
         
         @param width   new width
         @param height  new height
         */
        void set_size(float width, float height);
        
        /** Function for setting size.
         
         @param size    new width and height
         */
        void set_size(const Vec2<float>& size);
        
        /** Function for retrieving the position.
         
         @return    Current position
         */
        const Vec2<float>& get_position() const;
        
        /** Function for setting position.
         
         @param x   new x position
         @param y   new y position
         */
        void set_position(float x, float y);
        
        /** Function for setting position.
         
         @param position    new x and y positions
         */
        void set_position(const Vec2<float>& position);
        
        /** Function for moving.
         Will set the position based on the current values and
         add the offset.
         
         @param offset_x    amount to move x
         @param offset_y    amount to move y
         */
        void move(float offset_x, float offset_y);
        
        /** Function for moving.
         Will set the position based on the current values and
         add the offset.
         
         @param offset  amount to move x and y
         */
        void move(const Vec2<float>& offset);
        
    private:
        Vec2<float> m_size;
        Vec2<float> m_position;
    };
    
    // Comparison operators.
    bool operator==(const Widget& lhs, const Widget& rhs);
    bool operator!=(const Widget& lhs, const Widget& rhs);
}

#endif // PTK_CORE_WIDGET_HPP

