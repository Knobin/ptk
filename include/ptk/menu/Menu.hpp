//
//  menu/Menu.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2021-01-06.
//

#ifndef PTK_MENU_MENU_HPP
#define PTK_MENU_MENU_HPP

// Local Headers
#include "ptk/menu/MenuItemBase.hpp"
#include "ptk/Core.hpp"

// C++ Headers
#include <initializer_list>

namespace pTK
{
    class Menu : public MenuItemBase
    {
    public:
        /** typedefs for Menu.
        */
        using value_type                = Ref<MenuItemBase>;
        using container_type            = std::vector<value_type>;
        using allocator_type            = typename container_type::allocator_type;
        using reference                 = value_type&;
        using const_reference           = const value_type&;
        using pointer                   = typename std::allocator_traits<allocator_type>::pointer;
        using const_pointer             = typename std::allocator_traits<allocator_type>::const_pointer;
        using iterator                  = typename container_type::iterator;
        using reverse_iterator          = typename container_type::reverse_iterator;
        using const_iterator            = typename container_type::const_iterator;
        using const_reverse_iterator    = typename container_type::const_reverse_iterator;
        using size_type                 = typename container_type::size_type;
        using difference_type           = typename container_type::difference_type;

    public:
        explicit Menu(const std::string& name = "")
            : MenuItemBase(name), m_menuItems{}
        {

        }

        Menu(const std::string& name, const std::initializer_list<Ref<MenuItemBase>>& items)
            : MenuItemBase(name), m_menuItems{items}
        {

        }

        virtual ~Menu() = default;

        [[nodiscard]] std::string_view typeName() const override { return "Menu"; }

        void add(const Ref<MenuItemBase>& item)
        {
            m_menuItems.push_back(item);
        }

        /** Function for retrieving the size of the Container.

        */
        [[nodiscard]] size_type size() const
        {
            return m_menuItems.size();
        }

        /** Function for checking if the Container is empty.

            @return    empty
        */
        [[nodiscard]] bool empty() const
        {
            return m_menuItems.empty();
        }

        /** Function for retrieving the first element in the container.

            Should in no circumstances be called when the container is empty!
            This is considered to be undefined behavior!

            @return    first element in Container
        */
        reference front()
        {
            return m_menuItems.front();
        }

        /** Function for retrieving the first element in the container.

            Should in no circumstances be called when the container is empty!
            This is considered to be undefined behavior!

            @return    first element in Container
        */
        const_reference front() const
        {
            return m_menuItems.front();
        }

        /** Function for retrieving the last element in the container.

            Should in no circumstances be called when the container is empty!
            This is considered to be undefined behavior!

            @return    last element in container
        */
        reference back()
        {
            return m_menuItems.back();
        }

        /** Function for retrieving the last element in the container.

            Should in no circumstances be called when the container is empty!
            This is considered to be undefined behavior!

            @return    last element in container
        */
        const_reference back() const
        {
            return m_menuItems.back();
        }

        /** Function for retrieving the an iterator that points to the first
            value in the Container.

            The iterator may be equal to end iterator if the Container is empty.

            @return    iterator
        */
        iterator begin()
        {
            return m_menuItems.begin();
        }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the Container.

            The iterator should never be dereferenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    iterator
        */
        iterator end()
        {
            return m_menuItems.end();
        }

        /** Function for retrieving the an const iterator that points to the first
            value in the Container.

            The iterator may be equal to end iterator if the Container is empty.

            @return    const iterator
        */
        const_iterator cbegin() const
        {
            return m_menuItems.cbegin();
        }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the Container.

            The iterator should never be dereferenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        const_iterator cend() const
        {
            return m_menuItems.cend();
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
            return m_menuItems.rbegin();
        }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the Container.

            The iterator should never be dereferenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        reverse_iterator rend()
        {
            return m_menuItems.rend();
        }

    private:
        std::vector<Ref<MenuItemBase>> m_menuItems;
    };
}

#endif // PTK_MENU_MENU_HPP
