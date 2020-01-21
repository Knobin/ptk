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
#include "ptk/core/Sizable.hpp"
#include "ptk/util/Point.hpp"
#include "ptk/util/Alignment.hpp"

// C++ Headers
#include <string>

namespace pTK
{
    /** Widget class implementation.
     
     This class is low level class for widget, that
     has the essential component for rendering.
     */
    class Widget : public Drawable, public EventHandling, public Sizable
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
        void setSize(const Size& size) override;

        /** Function for requesting the const size of the Widget.
            
            @param size  requested const size of the Widget.
        */
        void setConstSize(const Size& size) override;
        
        /** Function for requesting the position of the Widget.
         
         @param pos  requested position of the Widget.
         */
        virtual void setPosHint(const Point& pos);
        
        /** Function for retrieving the current position of the Widget.
         
         @return  current position
         */
        const Point& getPosition() const;
        
        /** Function for setting the name of the Widget.
         
         @param  name   name of the widget
         */
        void setName(const std::string& name);
        
        /** Function for retrieving the name of the Widget.
         
         @return  name
         */
        const std::string& getName() const;
        
        /** Function for setting the margin of the Widget.
         
         @param margin  Margin to apply.
         */
        void setMargin(const Margin& margin);

        /** Function for setting the top margin of the Widget.
         
         @param topMargin  value to apply for top margin.
         */
        void setMarginTop(Margin::value_type topMargin);

        /** Function for setting the bottom margin of the Widget.
         
         @param bottomMargin  value to apply for bottom margin.
         */
        void setMarginBottom(Margin::value_type bottomMargin);

        /** Function for setting the left margin of the Widget.
         
         @param leftMargin  value to apply for left margin.
         */
        void setMarginLeft(Margin::value_type leftMargin);

        /** Function for setting the right margin of the Widget.
         
         @param rightMargin  value to apply for right margin.
         */
        void setMarginRight(Margin::value_type rightMargin);

        /** Function for setting the top and bottom margin of the Widget.
         
         @param topMargin  value to apply for top margin.
         @param bottomMargin  value to apply for bottom margin.
         */
        void setMarginTopBottom(Margin::value_type topMargin, Margin::value_type bottomMargin);

        /** Function for setting the left and right margin of the Widget.
         
         @param leftMargin  value to apply for left margin.
         @param rightMargin  value to apply for right margin.
         */
        void setMarginLeftRight(Margin::value_type leftMargin, Margin::value_type rightMargin);
        
        /** Function for retrieving the margin of the Widget.
         
         @return  current margin
         */
        const Margin& getMargin() const;

        /** Function for retrieving the top margin of the Widget.
         
         @return  current top margin
         */
        int32 getMarginTop() const;

        /** Function for retrieving the bottom margin of the Widget.
         
         @return  current bottom margin
         */
        int32 getMarginBottom() const;

        /** Function for retrieving the left margin of the Widget.
         
         @return  current left margin
         */
        int32 getMarginLeft() const;

        /** Function for retrieving the right margin of the Widget.
         
         @return  current right margin
         */
        int32 getMarginRight() const;

        void setAlign(std::underlying_type<Align>::type alignment);

        std::underlying_type<Align>::type getAlign() const;
        
    protected:
        /** Function for updating the child.
         
         */
        virtual bool updateChild(Widget*);
        
        /** Function for notifying the parent of a change and
         to put it on an internal render queue.
         */
        bool update();
        
        /** Function for redrawing the child.
         
         */
        virtual bool drawChild(Widget*);
        
        /** Function for notifying the parent of a change and
         put it on an internal render queue.
         */
        bool draw();
        
    private:
        Widget* m_parent;
        Point m_pos;
        std::string m_name;
        Margin m_margin;
        std::underlying_type<Align>::type m_align;
    };
    
    // Comparison operators.
    bool operator==(const Widget& lhs, const Widget& rhs);
    bool operator!=(const Widget& lhs, const Widget& rhs);
}

#endif // PTK_CORE_WIDGET_HPP

