//
//  menu/Menu.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENU_HPP
#define PTK_MENU_MENU_HPP

// pTK Headers
#include "ptk/menu/MenuItem.hpp"
#include "ptk/Core.hpp"

// C++ Headers
#include <vector>

namespace pTK
{
    /** Menu class implementation.

        Collection of menu items that forms a menu.
    */
    class PTK_API Menu : public MenuItem
    {
    public:
        using value_type = Ref<MenuItem>;
        using container_type = std::vector<value_type>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = typename container_type::iterator;
        using reverse_iterator = typename container_type::reverse_iterator;
        using const_iterator = typename container_type::const_iterator;
        using const_reverse_iterator = typename container_type::const_reverse_iterator;
        using size_type = typename container_type::size_type;

    public:
        /** Constructs Menu with name and items.

            @param name         displayed text
            @param items        optional list with menu items
            @return             initialized Menu
        */
        explicit Menu(const std::string& name, const std::initializer_list<Ref<MenuItem>>& items = {});

        /** Destructor.

        */
        virtual ~Menu() = default;

        /** Function for retrieving the name of the menu.

            @return name
        */
        [[nodiscard]] const std::string& name() const;

        /** Function for renaming the menu.

            @param name     new name of the menu
        */
        void rename(const std::string& name);

        /** Function for adding a menu item.

            @param item     menu item to add
        */
        void addItem(const Ref<MenuItem>& item);

        /** Function for removing a menu item.

            @param item     menu item to remove
        */
        void removeItem(const Ref<MenuItem>& item);

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

    private:
        container_type m_holder{};
        std::string m_name;
    };
}

#endif // PTK_MENU_MENU_HPP
