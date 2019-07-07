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

// C++ Headers
#include <vector>
#include <utility>
#include <functional>

namespace pTK
{
    /** Container class implementation.
     
     This class is a generic class with some common functions
     for holding and managing a certain value of T.
     */
    template <typename T>
    class Container
    {
    public:
        /** typedefs for Container.
         
         */
        using iterator = typename std::vector<T>::iterator;
        using reverse_iterator = typename std::vector<T>::reverse_iterator;
        using const_iterator = typename std::vector<T>::const_iterator;
        using const_reverse_iterator = typename std::vector<T>::const_reverse_iterator;
        
        /** Constructs Container with default values.
         
         @return    default initialized Container
         */
        Container()
            : m_holder{}
        {
        }
        
        /** Constructs Container with default values with item.
         
         @return    default initialized Container
         */
        Container(T item)
            : m_holder{}
        {
            insert(item);
        }
        
        /** Constructs Container with default values with list.
         
         @return    default initialized Container
         */
        Container(std::initializer_list<T> list)
        : m_holder{}
        {
            for (T item : list)
            {
                insert(item);
            }
        }
        
        /** Constructs Container with default values with Container.
         
         @return    default initialized Container
         */
        Container(const Container& rhs)
        {
            rhs.forEach([&](const T& item){
                insert(item);
            });
        }
        
        ~Container() = default;
        
        /** Container assignment operator.
         
         @param rhs     Container to copy from
         @return        Container with values from rhs
         */
        Container& operator=(const Container& rhs)
        {
            if (this == &rhs)
                return *this;
            
            rhs.forEach([&](const T& item){
                insert(item);
            });
            
            return *this;
        }
        
        /** Function for inserting an element to the Container.
         
         @param item        item to add
         */
        void insert(const T& item)
        {
            m_holder.push_back(item);
        }
        
        /** Function for removing an element from the Container.
         
         @param widget      widget to remove
         */
        void erase(const T& item)
        {
            const_iterator it = find(item);
            
            if (it != end())
                m_holder.erase(it);
        }
        
        /** Function for removing an element from the Container.
         
         @param widget      widget to remove
         */
        void erase(const_iterator it)
        {
            if (it != end())
                m_holder.erase(it);
        }
        
        /** Function for retrieving the element at the index in the Container.
         
         @param index   Index
         @return        element in Container
         */
        T at(uint index) const
        {
            if (index < 0 || index >= size())
                throw std::out_of_range("Index is out of range!");
            
            return m_holder.at(index);
        }
        
        /** Function for retrieving the size of the Container.
         
         */
        uint size() const
        {
            return m_holder.size();
        }
        
        /** Function for checking if the Container is empty.
         
         @return    empty
         */
        bool empty() const
        {
            return m_holder.empty();
        }
        
        /** Function for retrieving the first element in the container.
         
         Should in no circumstances be called when the container is empty!
         This is considered to be undefined behavior!
         
         @return    first element in Container
         */
        T front() const
        {
            return m_holder.front();
        }
        
        /** Function for retrieving the last element in the container.
         
         Should in no circumstances be called when the container is empty!
         This is considered to be undefined behavior!
         
         @return    last element in container
         */
        T back() const
        {
            return m_holder.back();
        }
        
        /** Function for retrieving the an iterator that points to the first
         value in the Container.
         
         The iterator may be equal to end iterator if the Container is empty.
         
         @return    iterator
         */
        iterator begin()
        {
            return m_holder.begin();
        }
        
        /** Function for retrieving the special iterator referring to
         the past-the-end of the Container.
         
         The iterator should never be dereferenced, due to the fact that the iterator
         does not point to a value and should therefore only be used for checking.
         
         @return    iterator
         */
        iterator end()
        {
            return m_holder.end();
            
        }
        
        /** Function for retrieving the an const iterator that points to the first
         value in the Container.
         
         The iterator may be equal to end iterator if the Container is empty.
         
         @return    const iterator
         */
        const_iterator cbegin() const
        {
            return m_holder.cbegin();
        }
        
        /** Function for retrieving the special const iterator referring to
         the past-the-end of the Container.
         
         The iterator should never be dereferenced, due to the fact that the iterator
         does not point to a value and should therefore only be used for checking.
         
         @return    const iterator
         */
        const_iterator cend() const
        {
            return m_holder.cend();
            
        }
        
        /** Function for retrieving the an iterator that points to the last
         value in the Container.
         
         This iterator is working in reverse. Meaning that is starts at the end
         and is moving to the beginning.
         
         The iterator may be equal to rend iterator if the Container is empty.
         
         @return    const iterator
         */
        reverse_iterator rbegin()
        {
            return m_holder.rbegin();
        }
        
        /** Function for retrieving the special const iterator referring to
         the past-the-end of the Container.
         
         The iterator should never be dereferenced, due to the fact that the iterator
         does not point to a value and should therefore only be used for checking.
         
         @return    const iterator
         */
        reverse_iterator rend()
        {
            return m_holder.rend();
        }
        
        /** Function for retrieving the first element in the Container that matches the parameters.
         
         Function will return the end iterator if the element is not found.
         
         @param item    item to find in Container
         @return        const iterator that points to the item.
         */
        iterator find(const T& item)
        {
            for (iterator it = begin(); it != end(); it++)
                if ((*it) == item)
                    return it;
            
            return end();
        }
        
        /** Function for retrieving the first element in the Container that matches the parameters
            starting from the last item and moving to the first.
         
         Function will return the end iterator if the element is not found.
         
         @param item    item to find in Container
         @return        const iterator that points to the item.
         */
        reverse_iterator rfind(const T& item)
        {
            for (reverse_iterator it = rbegin(); it != rend(); it++)
                if ((*it) == item)
                    return it;
            
            return rend();
        }
        
        /** Function for retrieving the first element in the Container that matches the parameters.
         
         Function will return the end iterator if the element is not found.
         
         @param cond    condition function
         @return        const iterator that points to the item.
         */
        iterator findIf(const std::function<bool(const T& item)>& cond)
        {
            for (iterator it = begin(); it != end(); it++)
                if (cond((*it)))
                    return it;
            
            return end();
        }
        
        /** Function for retrieving the first element in the Container that matches the parameters
         starting at the last element and moving to the start.
         
         Function will return the reverse end iterator if the element is not found.
         
         @param cond    condition function
         @return        const reverse iterator that points to the item.
         */
        reverse_iterator rfindIf(const std::function<bool(const T& item)>& cond)
        {
            for (reverse_iterator it = rbegin(); it != rend(); it++)
                if (cond((*it)))
                    return it;
            
            return rend();
        }
        
        /** Function for looping over the widgets in the container.
         
         @param func    function to call for each widget.
         */
        void forEach(const std::function<void(const T& widget)>& func) const
        {
            for (const_iterator it = cbegin(); it != cend(); it++)
                func((*it));
        }
        
    private:
        std::vector<T> m_holder;
    };
    
    // Comparison operators.
    template <typename T>
    bool operator==(const Container<T>& lhs, const Container<T>& rhs)
    {
        if (lhs.size() != rhs.size())
            return false;
        
        bool equal = true;
        int i = 0;
        lhs.forEach([&](const T& item){
            auto rhs_item = rhs.at(i);
            if (item != rhs_item)
                equal = false;
            i++;
        });
        
        return equal;
    }
    
    template <typename T>
    bool operator!=(const Container<T>& lhs, const Container<T>& rhs)
    {
        return !(lhs == rhs);
    }
}

#endif // PTK_CORE_CONTAINER_HPP

