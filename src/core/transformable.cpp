//
//  core/transformable.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

// Local Headers
#include "ptk/core/transformable.hpp"

namespace pTK
{
    Transformable::Transformable()
        : m_size{0, 0}, m_position{0 ,0}
    {
    }
    
    const Vec2<float>& Transformable::get_size() const
    {
        return m_size;
    }
    
    void Transformable::set_size(float width, float height)
    {
        m_size = {width, height};
    }
    
    void Transformable::set_size(const Vec2<float>& size)
    {
        m_size = size;
    }
    
    const Vec2<float>& Transformable::get_position() const
    {
        return m_position;
    }
    
    void Transformable::set_position(float x, float y)
    {
        m_position = {x, y};
    }
    
    void Transformable::set_position(const Vec2<float>& position)
    {
        m_position = position;
    }
    
    void Transformable::move(float offset_x, float offset_y)
    {
        m_position.x += offset_x;
        m_position.y += offset_y;
    }
    
    void Transformable::move(const Vec2<float>& offset)
    {
        m_position += offset;
    }
    
    // Comparison operators.
    bool operator==(const Transformable& lhs, const Transformable& rhs)
    {
        return ((lhs.get_position() == rhs.get_position()) && (lhs.get_size() == rhs.get_size()));
    }
    
    bool operator!=(const Transformable& lhs, const Transformable& rhs)
    {
        return !(lhs == rhs);
    }
}
