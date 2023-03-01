//
//  core/ApplicationBase.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2022-11-14.
//

#ifndef PTK_CORE_APPLICATIONBASE_HPP
#define PTK_CORE_APPLICATIONBASE_HPP

// pTK Headers
#include "ptk/util/SingleObject.hpp"

// C++ Headers
#include <cstdint>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace pTK
{
    // Forward declaration of Window class, it does not need to be known here.
    class Window;

    /** ApplicationBase class implementation.

        Contains the container for the window and common function for
        manipulation or viewing.

        Platform implementations should inherit this class.
    */
    class PTK_API ApplicationBase : public SingleObject
    {
    public:
        using key_type = int32_t;
        using mapped_type = Window*;
        using value_type = std::pair<key_type, mapped_type>;
        using container_type = std::vector<std::pair<key_type, mapped_type>>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = typename container_type::iterator;
        using reverse_iterator = typename container_type::reverse_iterator;
        using const_iterator = typename container_type::const_iterator;
        using const_reverse_iterator = typename container_type::const_reverse_iterator;
        using size_type = typename container_type::size_type;

    public:
        /** Deleted constructor, ApplicationBase must have an name.

        */
        ApplicationBase() = delete;

        /** Constructs ApplicationBase with name.

            @return        default initialized ApplicationBase with name
        */
        explicit ApplicationBase(std::string_view name);

        /** Destructor for ApplicationBase

        */
        virtual ~ApplicationBase() = default;

        /** Function for retrieving the name of the Application.

            @return    name of the application
        */
        [[nodiscard]] const std::string& name() const noexcept { return m_name; }

        /** Function for retrieving the an iterator that points to the first
            value in the Application.

            The iterator may be equal to end iterator if the Application is empty.

            @return    iterator
        */
        [[nodiscard]] iterator begin() noexcept { return m_holder.begin(); }

        /** Function for retrieving the an iterator that points to the first
            value in the Application.

            The iterator may be equal to end iterator if the Application is empty.

            @return    const iterator
        */
        [[nodiscard]] const_iterator begin() const noexcept { return m_holder.begin(); }

        /** Function for retrieving the an const iterator that points to the first
            value in the Application.

            The iterator may be equal to end iterator if the Application is empty.

            @return    const iterator
        */
        [[nodiscard]] const_iterator cbegin() const noexcept { return m_holder.cbegin(); }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the Application.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    iterator
        */
        [[nodiscard]] iterator end() noexcept { return m_holder.end(); }

        /** Function for retrieving the special iterator referring to
            the past-the-end of the Application.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        [[nodiscard]] const_iterator end() const noexcept { return m_holder.end(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the Application.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const iterator
        */
        [[nodiscard]] const_iterator cend() const noexcept { return m_holder.cend(); }

        /** Function for retrieving the an iterator that points to the last
            value in the Application.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the Application is empty.

            @return    reverse iterator
        */
        [[nodiscard]] reverse_iterator rbegin() noexcept { return m_holder.rbegin(); }

        /** Function for retrieving the an iterator that points to the last
            value in the Application.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the Application is empty.

            @return    const reverse begin iterator
        */
        [[nodiscard]] const_reverse_iterator rbegin() const noexcept { return m_holder.rbegin(); }

        /** Function for retrieving the an iterator that points to the last
            value in the Application.

            This iterator is working in reverse. Meaning that is starts at the end
            and is moving to the beginning.

            The iterator may be equal to rend iterator if the Application is empty.

            @return    const reverse begin iterator
        */
        [[nodiscard]] const_reverse_iterator crbegin() const noexcept { return m_holder.crbegin(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the Application.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    reverse reverse end iterator
        */
        [[nodiscard]] reverse_iterator rend() noexcept { return m_holder.rend(); }

        /** Function for retrieving the special const iterator referring to
            the past-the-end of the Application.

            The iterator should never be de-referenced, due to the fact that the iterator
            does not point to a value and should therefore only be used for checking.

            @return    const reverse end iterator
        */
        [[nodiscard]] const_reverse_iterator rend() const noexcept { return m_holder.rend(); }

        /** Function for retrieving the special const iterator referring to
             the past-the-end of the Application.

             The iterator should never be de-referenced, due to the fact that the iterator
             does not point to a value and should therefore only be used for checking.

             @return    const reverse end iterator
         */
        [[nodiscard]] const_reverse_iterator crend() const noexcept { return m_holder.crend(); }

        /** Function for finding the first window entry by key or window in the application.

            @param key      key to search for
            @param window   pointer to window to search for
            @return         iterator to entry, or cend()
        */
        [[nodiscard]] const_iterator firstByKeyOrWindow(int32_t key, Window* window) const;

        /** Function for finding the first window entry by key in the application.

            @param key      key to search for
            @return         iterator to entry, or cend()
        */
        [[nodiscard]] const_iterator firstByKey(int32_t key) const;

        /** Function for finding the first window entry by window pointer in the application.

            @param window   pointer to window to search for
            @return         iterator to entry, or cend()
        */
        [[nodiscard]] const_iterator firstByWindow(Window* window) const;

    protected:
        /** Function for inserting a key/window entry to the application.

            Note: Does not check the uniqueness of key or window pointer, use
                  uniqueInsert if a check for uniqueness is necessary before insertion.

            @param key      unique key
            @param window   pointer to window
            @return         true if added, otherwise false
        */
        bool insert(int32_t key, Window* window);

        /** Function for inserting a key/window entry to the application.

            Note: Checks if key or window pointer has already been inserted before
                  doing the insertion.

            @param key      unique key
            @param window   pointer to window
            @return         true if added, otherwise false
        */
        bool uniqueInsert(int32_t key, Window* window);

        /** Function for erasing a window entry from the application by key.

            @param key      unique key
            @return         true if erased, otherwise false
        */
        bool eraseByKey(int32_t key);

        /** Function for erasing a window entry from the application by window pointer.

            @param window   pointer to window
            @return         true if erased, otherwise false
        */
        bool eraseByWindow(Window* window);

        /** Function for erasing a window entry from the application by iterator.

            @param it       iterator to erase
            @return         true if erased, otherwise false
        */
        bool eraseByIter(const_iterator it);

        /** Function for retrieving the container.

            @return     const reference to window container
        */
        [[nodiscard]] const container_type& container() const noexcept { return m_holder; }

        /** Function for retrieving the container.

            @return     reference to window container
         */
        [[nodiscard]] container_type& container() noexcept { return m_holder; }

    private:
        container_type m_holder{};
        std::string m_name{};
    };
} // namespace pTK

#endif // PTK_CORE_APPLICATIONBASE_HPP
