//
//  core/Box.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-07-03.
//

#ifndef PTK_CORE_BOX_HPP
#define PTK_CORE_BOX_HPP

// Local Headers
#include "ptk/util/Container.hpp"
#include "ptk/widgets/Cell.hpp"
#include "ptk/util/Color.hpp"
#include "ptk/Log.hpp"

// C++ Headers
#include <string>
#include <memory>

namespace pTK
{
    using ContainerType = Container<std::shared_ptr<Cell>>;
    
    /** Box class implementation.
     
     Base class for handling a collection of Widgets.
     */
    class Box : public ContainerType, public Widget
    {
    public:
        Box();
        virtual ~Box() = default;
        
        /** Funtion for adding a widget to the Container.
         
         @param widget  widget to add
         */
        virtual bool add(const std::shared_ptr<Widget>& widget);
        
        /** Funtion for removing a widget in the Container.
         
         @param widget  widget to remove
         */
        virtual void remove(const std::shared_ptr<Widget>& widget);
        
        /** Funtion for finding a widget in the Container.
         Function will look for a widget with the specified name and it
         will grab the first with the matching name.
         
         Meaning that if two Widgets or more have the same name, only the
         first added will be found.
         
         @param name    name to find
         @return        const_iterator to widget or end const_iterator
         */
        const_iterator find(const std::string& name);
        
        /** Funtion for finding a widget in the Container.
         Function can be used to check if a pointer to Widget exists.
         
         @param widget  widget to find
         @return        const_iterator to widget or end const_iterator
         */
        const_iterator find(const Widget* widget);
        
        /** Funtion for finding a widget in the Container.
         
         @param pos     pos to find
         @return        const_iterator to widget or end const_iterator
         */
        const_iterator find(const Position& pos);
        
        /** Funtion for finding a widget in the Container.
         In reverse order.
         
         @param pos     pos to find
         @return        const_iterator to widget or end const_iterator
         */
        const_reverse_iterator rfind(const Position& pos);
        
        /** Draw function.
         Function is called when it is time to draw.
         
         Derived from Drawable.
         @param canvas  canvas to draw to
         */
        void onDraw(SkCanvas* canvas) override;
        
        // Event handling
        
        /** Function for handling when a key is pressed or released.
         
         Derived from EventFunctions.
         
         @param type    Key event (press or release)
         @param int     Keycode
         */
        bool onKeyEvent(EventType type, int32 keycode) override;
        
        /** Function for handling when mouse is hovering.
         
         Derived from EventFunctions.
         
         @param pos     position
         */
        bool onHoverEvent(const Position& pos) override;
        
        /** Function for handling when mouse is entering.
         
         Derived from EventFunctions.
         */
        bool onEnterEvent() override;
        
        /** Function for handling when mouse is leaving.
         
         Derived from EventFunctions.
         */
        bool onLeaveEvent() override;
        
        /** Function for handling when mouse is scrolling.
         
         Derived from EventFunctions.
         
         @param offset     x and y offset
         */
        bool onScrollEvent(const Vec2f& offset) override;
        
        /** Function for handling when mouse is clicking.
         
         Derived from EventFunctions.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool onClickEvent(MouseButton btn, const Position& pos) override;
        
        /** Function for handling when mouse is released.
         
         Derived from EventFunctions.
         
         @param button      which button on mouse triggered the event.
         @param position    x and y position
         */
        bool onReleaseEvent(MouseButton btn, const Position& pos) override;
        
        /** Funtion for setting the background of the Container.
         
         @param color   background color
         */
        void setBackground(const Color& color);
        
        /** Funtion for getting the background of the Container.
         
         @return    current background color
         */
        const Color& getBackground() const;
        
    protected:
        
        /** Funtion for getting the minimal size of all Cells in Container.
         
         @return    minimal size of all Cells
         */
        virtual Size calculateMinSize() const;
        
        /** Funtion for getting the miximal size of all Cells in Container.
         
         @return    miximal size of all Cells
         */
        virtual Size calculateMaxSize() const;
        
        /** Funtion for drawing all the Widgets in the Container.
         
         */
        void drawWidgets(SkCanvas* canvas);
        
    private:
        // These functions will not be used outside of this class.
        using ContainerType::push;
        using ContainerType::erase;
        
        // Variables
        Color m_background;
        Cell* m_lastClickedWidget;
        Cell* m_currentHoverWidget;
    };
}

#endif // PTK_CORE_BOX_HPP

