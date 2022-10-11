//
//  Application.hpp
//  pTK
//
//  Created by Robin Gustafsson on 2019-06-13.
//

#ifndef PTK_APPLICATION_HPP
#define PTK_APPLICATION_HPP

// pTK Headers
#include "ptk/Window.hpp"
#include "ptk/core/Types.hpp"
#include "ptk/util/SingleObject.hpp"

// C++ Headers
#include <map>
#include <utility>

namespace pTK
{
    /** Application class implementation.

        This class is to manage windows and handle the whole
        application.

    */
    class PTK_API Application : public SingleObject
    {
    public:
        using key_type = int32;
        using mapped_type = Window*;
        using value_type = std::pair<key_type, mapped_type>;
        using container_type = std::map<key_type, mapped_type>;
        using reference = value_type&;
        using const_reference = const value_type&;
        using iterator = typename container_type::iterator;
        using reverse_iterator = typename container_type::reverse_iterator;
        using const_iterator = typename container_type::const_iterator;
        using const_reverse_iterator = typename container_type::const_reverse_iterator;
        using size_type = typename container_type::size_type;

    public:
        /** Constructs Application with default values.

            @return        default initialized Application
        */
        Application() = delete;

        /** Constructs Application with default values.

            @param name    application name
            @return        default initialized Application
        */
        Application(const std::string& name);

        /** Constructs Event with default values.

            @param name    application name
            @param argc    argument count
            @param argv    arguments
            @return        default initialized Transformable
        */
        Application(const std::string& name, int argc, char* argv[]);

        /** Destructor for Application

        */
        virtual ~Application();

        /** Function for executing the application.

            @param window  start window
        */
        int exec(Window* window);

        /** Function for adding a window to the application.

            @param window   window
            @return         id
        */
        int32 addWindow(Window* window);

        /** Function for removing a window from the application with pointer.

            @param window   pointer to window
            @return         true if removed
        */
        bool removeWindow(Window* window);

        /** Function for removing a window from the application with id.

            @param key      window id
            @return         true if removed
        */
        bool removeWindow(int32 key);

        // TODO(knobin): Add docs.
        void removeAllWindows();

        // TODO(knobin): Add docs.
        [[nodiscard]] Window* findByKey(int32 key) const;

        /** Function for executing the application.

        */
        int run();

        // TODO(knobin): Add docs.
        void close();

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

            The iterator should never be dereferenced, due to the fact that the iterator
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

    public:
        static Application* Get();
        static Application* s_Instance;

    private:
        bool init(const std::string& name);

    private:
        container_type m_holder{};
    };
} // namespace pTK

#endif // PTK_APPLICATION_HPP
