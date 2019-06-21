//
//  core/container.cpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

// Local Headers
#include "ptk/core/container.hpp"
#include "ptk/log.hpp"

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
        rhs.for_each([&](const std::pair<std::shared_ptr<Widget>, int32_t>& pair){
            add(pair.first, pair.second);
        });
    }
    
    Container& Container::operator=(const Container& rhs)
    {
        if (this == &rhs)
            return *this;
        
        rhs.for_each([&](const std::pair<std::shared_ptr<Widget>, int32_t>& pair){
            add(pair.first, pair.second);
        });
        
        return *this;
    }
    
    void Container::add(const std::shared_ptr<Widget>& widget, int32_t priority)
    {
        if (widget != nullptr)
        {
            auto it = std::find_if(m_widgets.begin(), m_widgets.end(), [&](const std::pair<std::shared_ptr<Widget>, int32_t>& rhs){
                return widget == rhs.first;
            });
            if (it == m_widgets.end())
            {
                m_widgets.push_back(std::make_pair(widget, priority));
                sort();
            }
        }
    }
    
    void Container::remove(const std::shared_ptr<Widget>& widget)
    {
        auto it = std::remove_if(m_widgets.begin(), m_widgets.end(), [&](const std::pair<std::shared_ptr<Widget>, int32_t>& rhs){
            return widget == rhs.first;
        });
        m_widgets.erase(it, m_widgets.end());
    }
    
    const std::shared_ptr<Widget>& Container::at(uint32_t index) const
    {
        return m_widgets.at(index).first;
    }
    
    int Container::find_if(const std::function<bool(const std::shared_ptr<Widget>& widget)>& cond) const
    {
        for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
        {
            if (cond(it->first))
            {
                return std::distance(m_widgets.begin(), it);
            }
        }
        return -1;
    }
    
    int Container::find_if(const Vec2<float>& pos) const
    {
        for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
        {
            Vec2<float> w_pos = it->first->get_position();
            Vec2<float> w_size = it->first->get_size();
            if ((w_pos.x <= pos.x) && (w_pos.x + w_size.x >= pos.x))
            {
                if ((w_pos.y <= pos.y) && (w_pos.y + w_size.y >= pos.y))
                {
                    return std::distance(m_widgets.begin(), it);;
                }
            }
        }
        
        return -1;
    }
    
    uint32_t Container::count() const
    {
        return m_widgets.size();
    }
    
    const std::shared_ptr<Widget>& Container::front() const
    {
        return m_widgets.front().first;
    }
    
    const std::shared_ptr<Widget>& Container::back() const
    {
        return m_widgets.back().first;
    }
    
    void Container::set_priority(const std::shared_ptr<Widget>& widget, int32_t priority)
    {
        if (widget != nullptr)
        {
            auto it = std::find_if(m_widgets.begin(), m_widgets.end(), [&](const std::pair<std::shared_ptr<Widget>, int32_t>& rhs){
                return widget == rhs.first;
            });
            if (it != m_widgets.end())
            {
                it->second = priority;
                sort();
            }
        }
    }
    
    int32_t Container::get_priority(const std::shared_ptr<Widget>& widget) const
    {
        if (widget != nullptr)
        {
            auto it = std::find_if(m_widgets.begin(), m_widgets.end(), [&](const std::pair<std::shared_ptr<Widget>, int32_t>& rhs){
                return widget == rhs.first;
            });
            if (it != m_widgets.end())
            {
                return it->second;
            }
        }
        
        return 0;
    }
    
    void Container::for_each(const std::function<void(const std::shared_ptr<Widget>& widget)>& func) const
    {
        for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
        {
            func(it->first);
        }
    }
    
    void Container::for_each(const std::function<void(const std::pair<std::shared_ptr<Widget>, int32_t>)>& func) const
    {
        for (auto it = m_widgets.begin(); it != m_widgets.end(); it++)
        {
            func(*it);
        }
    }
    
    void Container::sort()
    {
        std::sort(m_widgets.begin(), m_widgets.end(), [](const std::pair<std::shared_ptr<Widget>, int32_t>& lhs, const std::pair<std::shared_ptr<Widget>, int32_t>& rhs){
            return lhs.second > rhs.second;
        });
    }
    
    // Comparison operators.
    bool operator==(const Container& lhs, const Container& rhs)
    {
        if (lhs.count() != rhs.count())
            return false;
        
        bool equal = true;
        int i = 0;
        lhs.for_each([&](const std::pair<std::shared_ptr<pTK::Widget>, int32_t>& pair){
            auto rhs_widget = rhs.at(i);
            if ((pair.first != rhs_widget) || (pair.second != rhs.get_priority(rhs_widget)))
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
