//
//  core/transformable.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_TRANSFORMABLE_HPP
#define PTK_CORE_TRANSFORMABLE_HPP

// Local Headers
#include "ptk/util/vec2.hpp"

// C++ Headers
#include <cstdint>

namespace pTK
{
    /** Transformable class implementation.
     
     This class is low level class for widget, that
     has the essential component for rendering.
     */
    class Transformable
    {
    public:
        /** Constructs Event with default values.
         
         @return    default initialized Transformable
         */
        Transformable();
        virtual ~Transformable() = default;
        
        /** Function for retrieving the size.
         
         @return    Current size
         */
        const Vec2<uint32_t>& get_size() const;
        
        /** Function for setting size.
         
         @param width   new width
         @param height  new height
         */
        void set_size(uint32_t width, uint32_t height);
        
        /** Function for setting size.
         
         @param size    new width and height
         */
        void set_size(const Vec2<uint32_t>& size);
        
        /** Function for retrieving the position.
         
         @return    Current position
         */
        const Vec2<uint32_t>& get_position() const;
        
        /** Function for setting position.
         
         @param x   new x position
         @param y   new y position
         */
        void set_position(uint32_t x, uint32_t y);
        
        /** Function for setting position.
         
         @param position    new x and y positions
         */
        void set_position(const Vec2<uint32_t>& position);
        
        /** Function for moving.
         Will set the position based on the current values and
         add the offset.
         
         @param offset_x    amount to move x
         @param offset_y    amount to move y
         */
        void move(uint32_t offset_x, uint32_t offset_y);
        
        /** Function for moving.
         Will set the position based on the current values and
         add the offset.
         
         @param offset  amount to move x and y
         */
        void move(const Vec2<uint32_t>& offset);
        
    private:
        Vec2<uint32_t> m_size;
        Vec2<uint32_t> m_position;
    };
    
    // Comparison operators.
    bool operator==(const Transformable& lhs, const Transformable& rhs);
    bool operator!=(const Transformable& lhs, const Transformable& rhs);
}

#endif // PTK_CORE_TRANSFORMABLE_HPP

