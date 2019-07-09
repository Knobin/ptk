//
//  core/Widget.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-19.
//

#ifndef PTK_CORE_WIDGET_HPP
#define PTK_CORE_WIDGET_HPP

// Local Headers
#include "ptk/core/Drawable.hpp"
#include "ptk/core/EventHandling.hpp"
#include "ptk/util/Size.hpp"
#include "ptk/util/Position.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    /** Widget class implementation.
     
     This class is low level class for widget, that
     has the essential component for rendering.
     */
    class Widget : public Drawable, public EventHandling
    {
    public:
        /** Constructs Event with default values.
         
         @return    default initialized Widget
         */
        Widget();
        virtual ~Widget() = default;
        
        /** Function sets the Container parent of the Widget.
         
         @param parent  the parent Container
         */
        void setParent(Widget* parent);
        
        /** Function for retrieving the Container parent of the Widget.
         
         @return  the Container parent of the Widget
         */
        Widget* getParent() const;
        
        /** Function for requesting the size of the Widget.
         
         @param size  requested size of the Widget.
         */
        virtual void setSizeHint(const Size& size);
        
        /** Function for requesting the position of the Widget.
         
         @param pos  requested position of the Widget.
         */
        virtual void setPosHint(const Position& pos);
        
        /** Function for retrieving the current size of the Widget.
         
         @return  current size
         */
        const Size& getSize() const;
        
        /** Function for retrieving the current position of the Widget.
         
         @return  current position
         */
        const Position& getPosition() const;
        
        /** Function for setting the name of the Widget.
         
         @param  name   name of the widget
         */
        void setName(const std::string& name);
        
        /** Function for retrieving the name of the Widget.
         
         @return  name
         */
        const std::string& getName() const;
        
    protected:
        /** Function for redrawing the child.
         
         */
        virtual bool drawChild(Widget*);
        
        /** Function for notifying the parent of a change and
         put it on an internal render queue.
         */
        bool draw();
        
    private:
        Widget* m_parent;
        
        Size m_size;
        Position m_pos;
        
        std::string m_name;
    };
    
    // Comparison operators.
    bool operator==(const Widget& lhs, const Widget& rhs);
    bool operator!=(const Widget& lhs, const Widget& rhs);
}

#endif // PTK_CORE_WIDGET_HPP

