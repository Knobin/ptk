//
//  core/Container.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

#ifndef PTK_CORE_CONTAINER_HPP
#define PTK_CORE_CONTAINER_HPP

// Local Headers
#include "ptk/core/Types.hpp"
#include "ptk/core/Widget.hpp"
#include "ptk/util/Color.hpp"

// C++ Headers
#include <vector>
#include <functional>
#include <algorithm>

namespace pTK
{
    /** Container class implementation.
     
     Base class for handling a collection of Widgets.
     */
    class Container : public Widget
    {
    public:
        /** typedefs for Container.
         */
        using type                      = std::shared_ptr<Widget>;
        using container_type            = std::vector<type>;
        using iterator                  = container_type::iterator;
        using reverse_iterator          = container_type::reverse_iterator;
        using const_iterator            = container_type::const_iterator;
        using const_reverse_iterator    = container_type::const_reverse_iterator;
        using size_type                 = uint;
        
        /** Constructs Container with default values.
         
         @return    default initialized Container
         */
        Container();
        
        /** Constructs Container with default values with item.
         
         @return    default initialized Container
         */
        Container(const type& item);
        
        /** Constructs Container with default values with list.
         
         @return    default initialized Container
         */
        Container(std::initializer_list<type> list);
        
        /** Constructs Container with default values with Container.
         
         @return    default initialized Container
         */
        Container(const Container& rhs);
        
        virtual ~Container();
        
        /** Container assignment operator.
         
         @param rhs     Container to copy from
         @return        Container with values from rhs
         */
        Container& operator=(const Container& rhs);
        
        /** Funtion for adding a widget to the Container.
         
         @param widget  widget to add
         */
        virtual bool add(const std::shared_ptr<Widget>& widget);
        
        /** Funtion for removing a widget in the Container.
         
         @param widget  widget to remove
         */
        virtual void remove(const std::shared_ptr<Widget>& widget);
        
        /** Function for retrieving the element at the index in the Container.
         
         @param index   Index
         @return        element in Container
         */
        type at(unsigned int index);
        
        /** Function for retrieving the element at the index in the Container.
         
         @param index   Index
         @return        element in Container
         */
        type at(unsigned int index) const;
        
        /** Operator for retrieving the element at the index in the Container.
         
         @param index   Index
         @return        element in Container
         */
        type operator[](unsigned int index) const;
        
        /** Function for retrieving the size of the Container.
         
         */
        size_type size() const;
        
        /** Function for checking if the Container is empty.
         
         @return    empty
         */
        bool empty() const;
        
        /** Function for retrieving the first element in the container.
         
         Should in no circumstances be called when the container is empty!
         This is considered to be undefined behavior!
         
         @return    first element in Container
         */
        type front() const;
        
        /** Function for retrieving the last element in the container.
         
         Should in no circumstances be called when the container is empty!
         This is considered to be undefined behavior!
         
         @return    last element in container
         */
        type back() const;
        
        /** Function for retrieving the an iterator that points to the first
         value in the Container.
         
         The iterator may be equal to end iterator if the Container is empty.
         
         @return    iterator
         */
        iterator begin();
        
        /** Function for retrieving the special iterator referring to
         the past-the-end of the Container.
         
         The iterator should never be dereferenced, due to the fact that the iterator
         does not point to a value and should therefore only be used for checking.
         
         @return    iterator
         */
        iterator end();
        
        /** Function for retrieving the an const iterator that points to the first
         value in the Container.
         
         The iterator may be equal to end iterator if the Container is empty.
         
         @return    const iterator
         */
        const_iterator cbegin() const;
        
        /** Function for retrieving the special const iterator referring to
         the past-the-end of the Container.
         
         The iterator should never be dereferenced, due to the fact that the iterator
         does not point to a value and should therefore only be used for checking.
         
         @return    const iterator
         */
        const_iterator cend() const;
        
        /** Function for retrieving the an iterator that points to the last
         value in the Container.
         
         This iterator is working in reverse. Meaning that is starts at the end
         and is moving to the beginning.
         
         The iterator may be equal to rend iterator if the Container is empty.
         
         @return    const iterator
         */
        reverse_iterator rbegin();
        
        /** Function for retrieving the special const iterator referring to
         the past-the-end of the Container.
         
         The iterator should never be dereferenced, due to the fact that the iterator
         does not point to a value and should therefore only be used for checking.
         
         @return    const iterator
         */
        reverse_iterator rend();
        
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
        reverse_iterator rfind(const Position& pos);
        
        /** Function for looping over the widgets in the container.
         
         @param func    function to call for each widget.
         */
        void forEach(const std::function<void(const type& item)>& func) const;
        
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
        
        /** Funtion for getting the minimal size of all childs in Container.
         
         @return    minimal size of all childs
         */
        virtual Size calculateMinSize() const;
        
        /** Funtion for getting the miximal size of all childs in Container.
         
         @return    miximal size of all childs
         */
        virtual Size calculateMaxSize() const;
        
        /** Funtion for drawing all the childs in the Container.
         
         */
        void drawChildren(SkCanvas* canvas);
        
    private:
        container_type m_holder;
        
        // Variables
        Color m_background;
        Widget* m_lastClickedWidget;
        Widget* m_currentHoverWidget;
    };
}

#endif // PTK_CORE_CONTAINER_HPP

