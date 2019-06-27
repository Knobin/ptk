//
//  core/Container.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

#ifndef PTK_CORE_CONTAINER_HPP
#define PTK_CORE_CONTAINER_HPP

// Local Headers
#include "ptk/core/Widget.hpp"

// C++ Headers
#include <vector>
#include <utility>
#include <functional>
#include <memory>

namespace pTK
{
    /** Container class implementation.
     
     This class is low level class for widgets to hold them
     together for easy access.
     
     The container is sorted by the widgets priority. Default
     priority is 0 and greater is better.
     */
    class Container
    {
    public:
        /** Constructs Container with default values.
         
         @return    default initialized Container
         */
        Container();
        
        /** Constructs Container with default values with Container.
         
         @return    default initialized Container
         */
        Container(const Container& rhs);
        
        virtual ~Container() = default;
        
        /** Container assignment operator.
         
         @param rhs     Container to copy from
         @return        Container with values from rhs
         */
        Container& operator=(const Container& rhs);
        
        /** Function for adding a widget to the container.
         
         @param widget      widget to add
         @param priority    priority of widget
         */
        virtual void add(const std::shared_ptr<Widget>& widget);
        
        /** Function for removing a widget to the container.
         
         @param widget      widget to remove
         */
        virtual void remove(const std::shared_ptr<Widget>& widget);
        
        /** Function for retrieving a widget at index in the container.
         
         @param index   index in container
         @return        widget in container
         */
        std::shared_ptr<Widget> at(uint index) const;
        
        /** Function for retrieving a Widget with the specified name.
         
         @param name    name of the Widget
         @return        Widget in container
         */
        std::shared_ptr<Widget> find(const std::string& name) const;
        
        /** Function for retrieving the first widget that matches the parameters.
         
         Function will return nullptr if no widget is found.
         
         @param cond    condition function
         @return        index where to find widget
         */
        std::shared_ptr<Widget> find_if(const std::function<bool(const std::shared_ptr<Widget>& widget)>& cond) const;
        
        /** Function for retrieving the first widget that matches the parameters. Starting at the end and moving to the front.
         
         Function will return nullptr if no widget is found.
         
         @param cond    condition function
         @return        index where to find widget
         */
        std::shared_ptr<Widget> rfind_if(const std::function<bool(const std::shared_ptr<Widget>& widget)>& cond) const;
        
        /** Function for retrieving the first widget that matches the parameters.
         
         Function will return nullptr if no widget is found.
         
         @param pos     position of widget to find
         @return        index where to find widget
         */
        std::shared_ptr<Widget> find_if(const Position& pos) const;
        
        /** Function for retrieving the first widget that matches the parameters.
         
         This function will begin checking with the last widget and iterate to
         the begining.
         
         Function will return nullptr if no widget is found.
         
         @param pos     position of widget to find
         @return        index where to find widget
         */
        std::shared_ptr<Widget> rfind_if(const Position& pos) const;
        
        /** Function for retrieving the first widget in the container.
         
         Should in no circumstances be called when the container is empty!
         This is considered to be undefined behavior!
         
         @return    first widget in container
         */
        std::shared_ptr<Widget> front() const;
        
        /** Function for retrieving the last widget in the container.
         
         Should in no circumstances be called when the container is empty!
         This is considered to be undefined behavior!
         
         @return    first last in container
         */
        std::shared_ptr<Widget> back() const;
        
        /** Function for removing a widget to the container.
         
         @return    widget count in container
         */
        uint count() const;
        
        /** Function for looping over the widgets in the container.
         
         @param func    function to call for each widget.
         */
        void for_each(const std::function<void(const std::shared_ptr<Widget>& widget)>& func) const;
        
        /** Function will be called when something changed in Widget
         that needs to be verified.
         
         @param widget  Child that needs to be verified
         @return        If change is accepted
         */
        virtual bool verifyChild(const Widget* widget);
        
    protected:
        /** Internal function for inserting a Widget.
         
         If widget is already inserted it will return false otherwise
         insert the Widget and return true.
         
         @param widget  Widget that wants to be added.
         @return        Status
         */
        bool insert_widget(const std::shared_ptr<Widget>& widget);
        
        /** Internal function for removing a Widget.
         
         If widget is not found, return will be false otherwise
         the widget will be removed and true will be returned.
         
         @param widget  Widget that wants to be removed.
         @return        Status
         */
        bool remove_widget(const std::shared_ptr<Widget>& widget);
        
    private:
        std::vector<std::shared_ptr<Widget>> m_widgets;
    };
    
    // Comparison operators.
    bool operator==(const Container& lhs, const Container& rhs);
    bool operator!=(const Container& lhs, const Container& rhs);
}

#endif // PTK_CORE_CONTAINER_HPP
