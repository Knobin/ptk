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
#include <functional>
#include <algorithm>

namespace pTK
{
    /** Container class implementation.
     
     Base class for handling a collection of items.
     */
    template<typename T> 
    class Container
    {
    public:
        /** typedefs for Container.
         */
        using type                      = T;
        using container_type            = std::vector<type>;
        using iterator                  = typename container_type::iterator;
        using reverse_iterator          = typename container_type::reverse_iterator;
        using const_iterator            = typename container_type::const_iterator;
        using const_reverse_iterator    = typename container_type::const_reverse_iterator;
        using size_type                 = size_t;
        
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
        Container(const type& item)
            : m_holder{}
        {
            add(item);
        }
        
        /** Constructs Container with default values with list.
         
         @return    default initialized Container
         */
        Container(std::initializer_list<type> list)
            : m_holder{}
        {
            m_holder.reserve(list.size());
            for (const type& item : list)
                add(item);
        }
        
        /** Constructs Container with default values with Container.
         
         @return    default initialized Container
         */
        Container(const Container& rhs)
        {
            m_holder.reserve(rhs.size());
            for (auto it{rhs.cbegin()}; it != rhs.cend(); it++)
                add((*it));
        }
        
        virtual ~Container() {}
        
        /** Container assignment operator.
         
         @param rhs     Container to copy from
         @return        Container with values from rhs
         */
        Container& operator=(const Container& rhs)
        {
            if (this == &rhs)
                return *this;
            
            m_holder.reserve(rhs.size());
            for (auto it{rhs.cbegin()}; it != rhs.cend(); it++)
                add((*it));
            
            return *this;
        }
        
        /** Funtion for adding a widget to the Container.
         
         @param item  item to add
         */
        virtual bool add(const type& item)
        {
            m_holder.push_back(item);
            return true;
        }
        
        /** Funtion for removing a widget in the Container.
         
         @param item  item to remove
         */
        virtual void remove(const type& item)
        {
            const_iterator it = find(item);
            if (it != cend())
                m_holder.erase(it);
        }

        /** Function for retrieving the element at the index in the Container.
         
         @param index   Index
         @return        element in Container
         */
        type at(unsigned int index)
        {
            if (index >= size())
                throw std::out_of_range("Index is out of range!");
            
            return m_holder.at(index);
        }
        
        /** Function for retrieving the element at the index in the Container.
         
         @param index   Index
         @return        element in Container
         */
        type at(unsigned int index) const
        {
            if (index >= size())
                throw std::out_of_range("Index is out of range!");
            
            return m_holder.at(index);
        }
        
        /** Operator for retrieving the element at the index in the Container.
         
         @param index   Index
         @return        element in Container
         */
        type operator[](unsigned int index) const
        {
            if (index >= size())
                throw std::out_of_range("Index is out of range!");
            
            return m_holder[index];
        }
        
        /** Function for retrieving the size of the Container.
         
         */
        size_type size() const
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
        type front() const
        {
            return m_holder.front();
        }
        
        /** Function for retrieving the last element in the container.
         
         Should in no circumstances be called when the container is empty!
         This is considered to be undefined behavior!
         
         @return    last element in container
         */
        type back() const
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

        /** Funtion for finding a widget in the Container.
         Function can be used to check if a pointer to Widget exists.
         
         @param widget  widget to find
         @return        const_iterator to widget or end const_iterator
         */
        const_iterator find(const type& item) const
        {
            for (auto it{cbegin()}; it != cend(); ++it)
                if ((*it) == item)
                    return it;
        
            return cend();
        }
        
        /** Function for looping over the items in the container.
         
         @param func    function to call for each item.
         */
        void forEach(const std::function<void(const type& item)>& func) const
        {
            for (const_iterator it = cbegin(); it != cend(); it++)
                func((*it));
        }
        
    private:
        container_type m_holder;
    };
}

#endif // PTK_CORE_CONTAINER_HPP

