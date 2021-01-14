//
//  util/IterableContainer.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-14.
//

#ifndef PTK_UTIL_ITERABLECONTAINER_HPP
#define PTK_UTIL_ITERABLECONTAINER_HPP

// Local Headers
#include "ptk/Core.hpp"

namespace pTK
{
    /** IterableContainer class implementation.

        Has basic container functionality.

        Override these callbacks if needed:
            -   void onAdd(const_reference item)
            -   void onRemove(const_reference item)
    */
    template<template <class,class> class Cont, typename T, typename A = std::allocator<T>>
    class IterableContainer
    {
    public:
        /** typedefs for IterableContainer.
        */
        using value_type = T;
        using container_type = Cont<value_type, A>;
        using container_type_reference = Cont<value_type, A> &;
        using allocator_type = typename container_type::allocator_type;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
        using iterator = typename container_type::iterator;
        using reverse_iterator = typename container_type::reverse_iterator;
        using const_iterator = typename container_type::const_iterator;
        using const_reverse_iterator = typename container_type::const_reverse_iterator;
        using size_type = typename container_type::size_type;
        using difference_type = typename container_type::difference_type;

        /** Constructs IterableContainer with default values.

            @return    default initialized IterableContainer
        */
        IterableContainer()
            : m_holder{}
        {

        }

        /** Constructs IterableContainer with default values with list.

            @return    default initialized IterableContainer
        */
        IterableContainer(const std::initializer_list<value_type>& list)
            : m_holder{}
        {
            m_holder.reserve(list.size());
            for (const value_type& item : list)
                add(item);
        }

        /** Constructs IterableContainer with default values with IterableContainer.

            @return    default initialized IterableContainer
        */
        IterableContainer(const IterableContainer &rhs)
        {
            m_holder.reserve(rhs.size());
            for (auto it{rhs.cbegin()}; it != rhs.cend(); it++)
                add((*it));
        }

        /** Deconstructor for IterableContainer.

        */
        virtual ~IterableContainer() = default;

        /** IterableContainer assignment operator.

            @param rhs     IterableContainer to copy from
            @return        IterableContainer with values from rhs
        */
        IterableContainer &operator=(const IterableContainer &rhs)
                {
            if (this == &rhs)
                return *this;

            m_holder.reserve(rhs.size());
            for (auto it{rhs.cbegin()}; it != rhs.cend(); it++)
                add((*it));

            return *this;
        }

        /** Function for adding an item to the IterableContainer.

            @param item  item to add
        */
        void add(const_reference item)
        {
            m_holder.push_back(item);
            onAdd(item);
        }

        /** Function for removing an item in the IterableContainer.
            Note: Does only remove first occurrence of the item (if multiple exists).

            @param item  item to remove
        */
        void remove(const_reference item)
        {
            const_iterator it{std::find(cbegin(), cend(), item)};
            if (it != cend())
            {
                onRemove(*it);
                m_holder.erase(it);
            }
        }

        /** Function for removing an item in the IterableContainer.

            @param it  iterator to item to remove
        */
        void erase(const_iterator it)
        {
            if (it != cend())
            {
                onRemove(*it);
                m_holder.erase(it);
            }
        }

        /** Function for retrieving the element at the index in the IterableContainer.

            @param pos      Index
            @return         element in IterableContainer
        */
        reference at(size_type pos)
        {
            if (pos >= size())
                throw std::out_of_range("Index is out of range!");

            return m_holder.at(pos);
        }

        /** Function for retrieving the element at the index in the IterableContainer.

            @param pos      Index
            @return         element in IterableContainer
        */
        const_reference at(size_type pos) const
        {
            if (pos >= size())
                throw std::out_of_range("Position is out of range!");

            return m_holder.at(pos);
        }

        /** Operator for retrieving the element at the index in the IterableContainer.

            @param index   Index
            @return        element in IterableContainer
        */
        reference operator[](size_type index)
        {
            return m_holder[index];
        }

        /** Operator for retrieving the element at the index in the IterableContainer.

            @param index   Index
            @return        element in IterableContainer
        */
        const_reference operator[](size_type index) const
        {
            return m_holder[index];
        }

        /** Function for retrieving the size of the IterableContainer.

        */
        size_type size() const noexcept
        {
            return m_holder.size();
        }

        /** Function for checking if the IterableContainer is empty.

            @return    empty
        */
        [[nodiscard]] bool empty() const noexcept
        {
            return m_holder.empty();
        }

        /** Function for retrieving the first element in the IterableContainer.

            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!

            @return    first element in IterableContainer
        */
        reference front()
        {
            return m_holder.front();
        }

        /** Function for retrieving the first element in the IterableContainer.

            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!

            @return    first element in IterableContainer
        */
        const_reference front() const
        {
            return m_holder.front();
        }

        /** Function for retrieving the last element in the IterableContainer.

            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!

            @return    last element in IterableContainer
        */
        reference back()
        {
            return m_holder.back();
        }

        /** Function for retrieving the last element in the IterableContainer.

            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!

            @return    last element in IterableContainer
        */
        const_reference back() const
        {
            return m_holder.back();
        }

        /** Function for retrieving the an iterator that points to the first
            value in the IterableContainer.

            The iterator may be equal to end iterator if the IterableContainer is empty.

            @return    iterator
        */
        iterator begin() noexcept
        {
            return m_holder.begin();
        }

        /** Function for retrieving the an iterator that points to the first
            value in the IterableContainer.

            The iterator may be equal to end iterator if the IterableContainer is empty.

            @return    const iterator
        */
        const_iterator begin() const noexcept
        {
            return m_holder.begin();
        }

        /** Function for retrieving the an const iterator that points to the first
            value in the IterableContainer.

            The iterator may be equal to end iterator if the IterableContainer is empty.

            @return    const iterator
        */
        const_iterator cbegin() const noexcept
        {
            return m_holder.cbegin();
        }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    iterator
        */
        iterator end() noexcept
        {
            return m_holder.end();
        }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        const_iterator end() const noexcept
        {
            return m_holder.end();
        }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be dereferenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        const_iterator cend() const noexcept
        {
            return m_holder.cend();
        }

        /** Function for retrieving the an iterator that points to the last
            value in the IterableContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the IterableContainer is empty.

            @return    reverse iterator
        */
        reverse_iterator rbegin() noexcept
        {
            return m_holder.rbegin();
        }

        /** Function for retrieving the an iterator that points to the last
            value in the IterableContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the IterableContainer is empty.

            @return    const reverse begin iterator
        */
        const_reverse_iterator rbegin() const noexcept
        {
            return m_holder.rbegin();
        }

        /** Function for retrieving the an iterator that points to the last
            value in the IterableContainer.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the IterableContainer is empty.

            @return    const reverse begin iterator
        */
        const_reverse_iterator crbegin() const noexcept
        {
            return m_holder.crbegin();
        }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    reverse reverse end iterator
        */
        reverse_iterator rend() noexcept
        {
            return m_holder.rend();
        }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the IterableContainer.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const reverse end iterator
        */
        const_reverse_iterator rend() const noexcept
        {
            return m_holder.rend();
        }

        /** Function for retrieving the special const iterator referring to
             the past-the-end of the IterableContainer.

             The iterator should never be de-referenced, due to the fact that the iterator
             does not point to a value and should therefore only be used for checking.

             @return    const reverse end iterator
         */
        const_reverse_iterator crend() const noexcept
        {
            return m_holder.crend();
        }

        /** Function for retrieving the data pointer.

            @return    pointer to data
        */
        value_type* data() noexcept
        {
            return m_holder.data();
        }

        /** Function for retrieving the data pointer.

            @return    const pointer to data
        */
        const value_type* data() const noexcept
        {
            return m_holder.data();
        }

        /** Function for retrieving the container.

            @return    const pointer to data
        */
        container_type_reference container() const noexcept
        {
            return m_holder;
        }

    private:
        /** Callback for when an item has been added to the container.
            Note: Will only be called if add() is used.

            @param item   added item
        */
        virtual void onAdd(const_reference UNUSED(item)) {}

        /** Callback for when an item is about to be removed from the container.
            Note: Will only be called if remove() or erase is used.

            @param item   removed item
        */
        virtual void onRemove(const_reference UNUSED(item)) {}

    private:
        container_type m_holder;
    };
}

#endif // PTK_UTIL_ITERABLECONTAINER_HPP
