//
//  menu/MenuBar.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENUBAR_HPP
#define PTK_MENU_MENUBAR_HPP

// pTK Headers
#include "ptk/menu/Menu.hpp"
#include "ptk/menu/MenuItem.hpp"

// C++ Headers
#include <memory>
#include <vector>

namespace pTK
{
    /** MenuBar class implementation.

        Collection of menus that forms a menu bar.
    */
    class PTK_API MenuBar
    {
    public:
        using value_type = std::shared_ptr<Menu>;
        using container_type = std::vector<value_type>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = typename container_type::iterator;
        using reverse_iterator = typename container_type::reverse_iterator;
        using const_iterator = typename container_type::const_iterator;
        using const_reverse_iterator = typename container_type::const_reverse_iterator;
        using size_type = typename container_type::size_type;

    public:
        /** Constructs Menu with default values.

            @return     default initialized Menu
        */
        MenuBar() = default;

        /** Constructs Menu with menus.

            @param menus    collection of menus to add
            @return         initialized Menu
        */
        MenuBar(const std::initializer_list<value_type>& menus);

        /** Destructor.

        */
        virtual ~MenuBar() = default;

        /** Function for adding a menu.

            @param menu     menu to add
        */
        void addMenu(const value_type& menu);

        /** Function for removing a menu.

            @param menu     menu to remove
        */
        void removeMenu(const value_type& menu);

        /** Function for checking if the menu bar is empty.

            @return     true if empty, otherwise false
        */
        [[nodiscard]] bool empty() const;

        /** Function for retrieving the an iterator that points to the first
            value in the MenuBar.

            The iterator may be equal to end iterator if the MenuBar is empty.

            @return    iterator
        */
        [[nodiscard]] iterator begin() noexcept { return m_holder.begin(); }

        /** Function for retrieving the an iterator that points to the first
            value in the MenuBar.

            The iterator may be equal to end iterator if the MenuBar is empty.

            @return    const iterator
        */
        [[nodiscard]] const_iterator begin() const noexcept { return m_holder.begin(); }

        /** Function for retrieving the an const iterator that points to the first
            value in the MenuBar.

            The iterator may be equal to end iterator if the MenuBar is empty.

            @return    const iterator
        */
        [[nodiscard]] const_iterator cbegin() const noexcept { return m_holder.cbegin(); }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the MenuBar.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    iterator
        */
        [[nodiscard]] iterator end() noexcept { return m_holder.end(); }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the MenuBar.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        [[nodiscard]] const_iterator end() const noexcept { return m_holder.end(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the MenuBar.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        [[nodiscard]] const_iterator cend() const noexcept { return m_holder.cend(); }

        /** Function for retrieving the an iterator that points to the last
            value in the MenuBar.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the MenuBar is empty.

            @return    reverse iterator
        */
        [[nodiscard]] reverse_iterator rbegin() noexcept { return m_holder.rbegin(); }

        /** Function for retrieving the an iterator that points to the last
            value in the MenuBar.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the MenuBar is empty.

            @return    const reverse begin iterator
        */
        [[nodiscard]] const_reverse_iterator rbegin() const noexcept { return m_holder.rbegin(); }

        /** Function for retrieving the an iterator that points to the last
            value in the MenuBar.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the MenuBar is empty.

            @return    const reverse begin iterator
        */
        [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return m_holder.crbegin(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the MenuBar.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    reverse reverse end iterator
        */
        [[nodiscard]] reverse_iterator rend() noexcept { return m_holder.rend(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the MenuBar.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const reverse end iterator
        */
        [[nodiscard]] const_reverse_iterator rend() const noexcept { return m_holder.rend(); }

        /** Function for retrieving the special const iterator referring to
             the past-the-end of the MenuBar.

             The iterator should never be de-referenced, due to the fact that the iterator
             does not point to a value and should therefore only be used for checking.

             @return    const reverse end iterator
         */
        [[nodiscard]] const_reverse_iterator crend() const noexcept { return m_holder.crend(); }

    private:
        container_type m_holder{};
    };
} // namespace pTK

#endif // PTK_MENU_MENUBAR_HPP
