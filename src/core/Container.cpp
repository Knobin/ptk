//
//  core/Container.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

// Local Headers
#include "ptk/core/Container.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <algorithm>

namespace pTK
{
    Container::Container()
        : m_widgets{}
    {
    }
    
    Container::Container(const Container& rhs)
    {
        rhs.for_each([&](const std::shared_ptr<Widget>& widget){
            add(widget);
        });
    }
    
    Container& Container::operator=(const Container& rhs)
    {
        if (this == &rhs)
            return *this;
        
        rhs.for_each([&](const std::shared_ptr<Widget>& widget){
            add(widget);
        });
        
        return *this;
    }
    
    void Container::add(const std::shared_ptr<Widget>& widget)
    {
        insert_widget(widget);
    }
    
    void Container::remove(const std::shared_ptr<Widget>& widget)
    {
        remove_widget(widget);
    }
    
    std::shared_ptr<Widget> Container::at(uint32_t index) const
    {
        if (index < count())
            return m_widgets.at(index);
        
        return std::shared_ptr<Widget>(nullptr);
    }
    
    std::shared_ptr<Widget> Container::find_if(const std::function<bool(const std::shared_ptr<Widget>& widget)>& cond) const
    {
        for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
            if (cond((*it)))
                return (*it);
        
        return std::shared_ptr<Widget>(nullptr);
    }
    
    std::shared_ptr<Widget> Container::find_if(const Vec2<float>& pos) const
    {
        for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
        {
            Vec2<float> w_pos = (*it)->getPosition();
            Vec2<float> w_size = (*it)->getSize();
            if ((w_pos.x <= pos.x) && (w_pos.x + w_size.x >= pos.x))
                if ((w_pos.y <= pos.y) && (w_pos.y + w_size.y >= pos.y))
                    return (*it);
        }
        
        return std::shared_ptr<Widget>(nullptr);
    }
    
    std::shared_ptr<Widget> Container::rfind_if(const Vec2<float>& pos) const
    {
        for (auto it = m_widgets.rbegin(); it != m_widgets.rend(); it++)
        {
            Vec2<float> w_pos = (*it)->getPosition();
            Vec2<float> w_size = (*it)->getSize();
            if ((w_pos.x <= pos.x) && (w_pos.x + w_size.x >= pos.x))
                if ((w_pos.y <= pos.y) && (w_pos.y + w_size.y >= pos.y))
                    return (*it);
        }
        
        return std::shared_ptr<Widget>(nullptr);
    }
    
    uint32_t Container::count() const
    {
        return m_widgets.size();
    }
    
    std::shared_ptr<Widget> Container::front() const
    {
        return m_widgets.front();
    }
    
    std::shared_ptr<Widget> Container::back() const
    {
        return m_widgets.back();
    }
    
    void Container::for_each(const std::function<void(const std::shared_ptr<Widget>& widget)>& func) const
    {
        for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
            func((*it));
    }
    
    bool Container::verifyChild(Widget*) const
    {
        return true;
    }
    
    bool Container::insert_widget(const std::shared_ptr<Widget>& widget)
    {
        if (widget != nullptr)
        {
            auto it = std::find_if(m_widgets.begin(), m_widgets.end(), [&](const std::shared_ptr<Widget>& rhs){
                return widget == rhs;
            });
            if (it == m_widgets.end())
            {
                m_widgets.push_back(widget);
                widget->setParent(this);
                return true;
            }
        }
        
        return false;
    }
    
    bool Container::remove_widget(const std::shared_ptr<Widget>& widget)
    {
        auto it = std::remove_if(m_widgets.begin(), m_widgets.end(), [&](const std::shared_ptr<Widget>& rhs){
            return widget == rhs;
        });
        if (it != m_widgets.end())
        {
            m_widgets.erase(it, m_widgets.end());
            widget->setParent(nullptr);
            return true;
        }
        
        return false;
    }
    
    // Comparison operators.
    bool operator==(const Container& lhs, const Container& rhs)
    {
        if (lhs.count() != rhs.count())
            return false;
        
        bool equal = true;
        int i = 0;
        lhs.for_each([&](const std::shared_ptr<pTK::Widget>& widget){
            auto rhs_widget = rhs.at(i);
            if (widget != rhs_widget)
                equal = false;
            i++;
        });
        
        return equal;
    }
    
    bool operator!=(const Container& lhs, const Container& rhs)
    {
        return !(lhs == rhs);
    }
}
