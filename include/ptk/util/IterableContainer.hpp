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

// C++ Headers
#include <vector>
#include <map>

namespace pTK
{
    template<typename Container>
    class IterableContainerBase
    {
    public:
        /** typedefs for IterableContainerBase.
        */
        using value_type = typename Container::value_type;
        using allocator_type = typename Container::allocator_type;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
        using iterator = typename Container::iterator;
        using reverse_iterator = typename Container::reverse_iterator;
        using const_iterator = typename Container::const_iterator;
        using const_reverse_iterator = typename Container::const_reverse_iterator;
        using size_type = typename Container::size_type;
        using difference_type = typename Container::difference_type;
        
    public:
        IterableContainerBase() = default;
        ~IterableContainerBase() = default;
        
        IterableContainerBase(const IterableContainerBase& rhs)
        {
            m_holder = rhs.container();
        }
        
        /** IterableContainer assignment operator.

            @param rhs     IterableContainer to copy from
            @return        IterableContainer with values from rhs
        */
        IterableContainerBase &operator=(const IterableContainerBase& rhs)
        {
            if (this == &rhs)
                return *this;

            swap(rhs);

            return *this;
        }
        
        void swap(IterableContainerBase& other) noexcept
        {
            m_holder.swap(other);
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
        
        /** Function for retrieving the container.

            @return    const pointer to data
        */
        Container& container() noexcept
        {
            return m_holder;
        }
        
        /** Function for retrieving the container.

            @return    const pointer to data
        */
        const Container& container() const noexcept
        {
            return m_holder;
        }
        
    private:
        Container m_holder;
    };

    /** IterableContainer class implementation.

        Has basic container functionality.

        Override these callbacks if needed:
            -   void onAdd(const_reference item)
            -   void onRemove(const_reference item)
    */
    template<typename Key, typename T, typename Compare = std::less<Key>, typename Allocator = std::allocator<std::pair<const Key, T>>>
    class IterableAssociative : public IterableContainerBase<std::map<Key, T, Compare, Allocator>>
    {
    public:
        /** typedefs for IterableContainer.
        */
        using container_type = std::map<Key, T, Compare, Allocator>;
        using base_type = IterableContainerBase<container_type>;
        using value_type = typename base_type::value_type;
        using allocator_type = typename base_type::allocator_type;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
        using iterator = typename base_type::iterator;
        using reverse_iterator = typename base_type::reverse_iterator;
        using const_iterator = typename base_type::const_iterator;
        using const_reverse_iterator = typename base_type::const_reverse_iterator;
        using size_type = typename base_type::size_type;
        using difference_type = typename base_type::difference_type;
        
    public:
        /** Constructs IterableContainer with default values.

            @return    default initialized IterableContainer
        */
        IterableAssociative() = default;

        /** Deconstructor for IterableContainer.

        */
        virtual ~IterableAssociative() = default;
        
        /** Function for retrieving the element at the index in the IterableContainer.

            @param pos      Index
            @return         element in IterableContainer
        */
        T& at(const Key& key)
        {
            return base_type::container().at(key);
        }

        /** Function for retrieving the element at the index in the IterableContainer.

            @param pos      Index
            @return         element in IterableContainer
        */
        const T& at(const Key& key) const
        {
            return base_type::container().at(key);
        }
    };

    /** IterableContainer class implementation.

        Has basic container functionality.

        Override these callbacks if needed:
            -   void onAdd(const_reference item)
            -   void onRemove(const_reference item)
    */
    template<typename T, typename A = std::allocator<T>>
    class IterableSequence : public IterableContainerBase<std::vector<T, A>>
    {
    public:
        /** typedefs for IterableContainer.
        */
        using container_type = std::vector<T, A>;
        using base_type = IterableContainerBase<container_type>;
        using value_type = typename base_type::value_type;
        using allocator_type = typename base_type::allocator_type;
        using reference = value_type &;
        using const_reference = const value_type &;
        using pointer = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer = typename std::allocator_traits<allocator_type>::const_pointer;
        using iterator = typename base_type::iterator;
        using reverse_iterator = typename base_type::reverse_iterator;
        using const_iterator = typename base_type::const_iterator;
        using const_reverse_iterator = typename base_type::const_reverse_iterator;
        using size_type = typename base_type::size_type;
        using difference_type = typename base_type::difference_type;
        
    public:
        /** Constructs IterableContainer with default values.

            @return    default initialized IterableContainer
        */
        IterableSequence() = default;

        /** Constructs IterableContainer with default values with list.

            @return    default initialized IterableContainer
        */
        IterableSequence(const std::initializer_list<T>& list)
            : base_type()
        {
            auto& cont = base_type::container();
            base_type::container().reserve(list.size());
            for (const value_type& item : list)
                cont.push_back(item);
        }

        /** Deconstructor for IterableContainer.

        */
        virtual ~IterableSequence() = default;

        /** Function for retrieving the element at the index in the IterableContainer.

            @param pos      Index
            @return         element in IterableContainer
        */
        reference at(size_type pos)
        {
            auto& cont = base_type::container();
            if (pos >= cont.size())
                throw std::out_of_range("Index is out of range!");

            return cont.at(pos);
        }

        /** Function for retrieving the element at the index in the IterableContainer.

            @param pos      Index
            @return         element in IterableContainer
        */
        const_reference at(size_type pos) const
        {
            const auto& cont = base_type::container();
            if (pos >= cont.size())
                throw std::out_of_range("Position is out of range!");

            return cont.at(pos);
        }

        /** Operator for retrieving the element at the index in the IterableContainer.

            @param index   Index
            @return        element in IterableContainer
        */
        reference operator[](size_type index)
        {
            return base_type::container()[index];
        }

        /** Operator for retrieving the element at the index in the IterableContainer.

            @param index   Index
            @return        element in IterableContainer
        */
        const_reference operator[](size_type index) const
        {
            return base_type::container()[index];
        }

        /** Function for retrieving the first element in the IterableContainer.

            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!

            @return    first element in IterableContainer
        */
        reference front()
        {
            return base_type::container().front();
        }

        /** Function for retrieving the first element in the IterableContainer.

            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!

            @return    first element in IterableContainer
        */
        const_reference front() const
        {
            return base_type::container().front();
        }

        /** Function for retrieving the last element in the IterableContainer.

            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!

            @return    last element in IterableContainer
        */
        reference back()
        {
            return base_type::container().back();
        }

        /** Function for retrieving the last element in the IterableContainer.

            Should in no circumstances be called when the IterableContainer is empty!
            This is considered to be undefined behavior!

            @return    last element in IterableContainer
        */
        const_reference back() const
        {
            return base_type::container().back();
        }
    };
}

#endif // PTK_UTIL_ITERABLECONTAINER_HPP
