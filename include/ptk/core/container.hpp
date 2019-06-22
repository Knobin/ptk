//
//  core/container.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-20.
//

#ifndef PTK_CORE_CONTAINER_HPP
#define PTK_CORE_CONTAINER_HPP

// Local Headers
#include "ptk/core/widget.hpp"

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
        void add(const std::shared_ptr<Widget>& widget);
        
        /** Function for removing a widget to the container.
         
         @param widget      widget to remove
         */
        void remove(const std::shared_ptr<Widget>& widget);
        
        /** Function for retrieving a widget at index in the container.
         
         @param index   index in container
         @return        widget in container
         */
        std::shared_ptr<Widget> at(uint32_t index) const;
        
        /** Function for retrieving the first widget that matches the parameters.
         
         Function will return nullptr if no widget is found.
         
         @param pos     position of widget to find
         @return        index where to find widget
         */
        std::shared_ptr<Widget> find_if(const Vec2<float>& pos) const;
        
        /** Function for retrieving the first widget that matches the parameters.
         
         This function will begin checking with the last widget and iterate to
         the begining.
         
         Function will return nullptr if no widget is found.
         
         @param pos     position of widget to find
         @return        index where to find widget
         */
        std::shared_ptr<Widget> rfind_if(const Vec2<float>& pos) const;
        
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
        uint32_t count() const;
        
        /** Function for looping over the widgets in the container.
         
         @param func    function to call for each widget.
         */
        void for_each(const std::function<void(const std::shared_ptr<Widget>& widget)>& func) const;
        
    private:
        std::vector<std::shared_ptr<Widget>> m_widgets;
    };
    
    // Comparison operators.
    bool operator==(const Container& lhs, const Container& rhs);
    bool operator!=(const Container& lhs, const Container& rhs);
}

#endif // PTK_CORE_CONTAINER_HPP
