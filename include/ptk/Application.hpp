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
#include "ptk/core/ApplicationBase.hpp"

// Temp.
#include "ptk/platform/base/ApplicationHandle.hpp"

namespace pTK
{
    /** Application class implementation.

        This class is for adding/removing windows and to
        setup the Application and its runtime environment.
    */
    class PTK_API Application : public ApplicationBase
    {
    public:
        /** Constructs Application with default values.

            @return        default initialized Application
        */
        Application() = delete;

        /** Constructs Application with default values.

            @param name    application name
            @return        default initialized Application
        */
        explicit Application(std::string_view name);

        /** Constructs Event with default values.

            @param name    application name
            @param argc    argument count
            @param argv    arguments
            @return        default initialized Transformable
        */
        Application(std::string_view name, int argc, char* argv[]);

        /** Destructor for Application

        */
        virtual ~Application();

        /** Function for executing the application with a single window.

            The Window should not have been added with addWindow() before this call.

            @param window  start window
        */
        int exec(Window* window);

        /** Function for adding a window to the application.

            @param window   window
            @return         id
        */
        key_type addWindow(Window* window);

        /** Function for removing a window from the application with pointer.

            @param window   pointer to window
            @return         true if removed
        */
        bool removeWindow(Window* window);

        /** Function for removing a window from the application with id.

            @param key      window id
            @return         true if removed
        */
        bool removeWindow(key_type key);

        /** Function for removing all windows in the application.

            Internally calls removeWindow() on all windows.
        */
        void clearWindows();

        /** Function for closing all windows in the application.

            Will close all windows, they might be removed from the application.
            It is up to the Window implementation.
        */
        void closeWindows();

        /** Function for executing the application.

            Runs the application with the already added windows.
            Note: Windowless application is not currently supported.

            @return     status
        */
        int run();

        /** Function for closing the application.

            Note: Should only be called if the application should be
                  stopped but not destructed yet.

            @return     closing status, true if closing, false if halted
        */
        bool close();

        /** Function for terminating the application.

            Application will close and exit.
            Use this function to terminate the whole applicaion.

            Further execution beyond this function may or may not be
            possible depending on the platform implementation.
        */
        void terminate();

        /** Function for retrieving if the application is closed.

            @return     closed status
        */
        [[nodiscard]] bool isClosed() const noexcept { return m_closed; }

        /** Function for allowing the application to run in headless mode.

            @param value    allowed to run in headless mode
        */
        void allowHeadless(bool value) noexcept { m_allowHeadless = value; }

        /** Function for retrieving if the application is allowed to run in headless mode.

            @return     status
        */
        [[nodiscard]] bool isHeadlessAllowed() const noexcept { return m_allowHeadless; }

        /** Function for retrieving if the application is running in headless mode.

            @return     status
        */
        [[nodiscard]] bool isRunningHeadless() const noexcept { return m_runningHeadless; }

        /** Function for allowing the application to terminate in destructor.

            @param value    allowed to terminate in destructor
        */
        void allowAutoTermination(bool value) noexcept { m_allowAutoTermination = value; }

        /** Function for retrieving if the application is allowed to terminate in destructor.

            @return     status
        */
        [[nodiscard]] bool isAutoTerminationAllowed() const noexcept { return m_allowAutoTermination; }

        /** Function for retrieving if the application is terminated.

            @return     terminated status
        */
        [[nodiscard]] bool isTerminated() const noexcept { return m_terminated; }

    public:
        /** Function for retrieving a pointer to the Application.

            Should only be called after the Application has been created.
            Otherwise, no instance exists and is nullptr.

            @return     pointer to application instance
        */
        static Application* Get();

    private:
        /** Callback for when the application will begin to close.

            If the closing of the application should be halted for whatever
            reason (some confirmation "are you sure?" for example) return
            false in this function.

            Note: This function must eventually return true for the application
                  to be able to close.

            @return     if the application should close
        */
        virtual bool onCloseRequest() { return true; }

        /** Callback for when the application is closing.

            Application is closing here, to halt the stopping use the
            onCloseRequest callback.
        */
        virtual void onClose() {}

    private:
        /** Function for initializing the Application.

            Should only be called once when the Application has been created.
            More than one call to this function will result in a ApplicationError
            exception being thrown.

            @param name     application name
            @return         status
        */
        bool init(std::string_view name);

        /** Helper function for removing a window from the Application.

            Calls the callback for removing a window and the removes it.

            @param it   iterator to be erased
        */
        void eraseWindow(const_iterator it);

        /** Message Loop for Headless mode.

            - Requires headless mode be allowed.
            - Windows should not be present in the application.
                - Debug will assert, Release will work but Windows wont handle events.

            @return     status
        */
        int headlessMessageLoop();

        /** Message Loop for standard mode.

            - Requires that at least one window has been added.

            @return     status
        */
        int standardMessageLoop();

        /** Function for selecting the function for event polling.

            Will select between pollEvents, waitEvents & waitEventsTimeout
            depending on certain conditions.

            For example:
                - pollEvents: if we need to check for an event and cannot wait.
                - waitEvents: if we can wait for an event indefinitely.
                - waitEventsTimeout: if we have some work that must run every x time
                                     but fine waiting before running that work again.
        */
        void fetchEvents();

        // Calls all closing functions.
        void closeHelper();

    private:
        static Application* s_Instance;
        Platform::ApplicationHandle* m_handle{nullptr};
        bool m_allowHeadless{false};
        bool m_runningHeadless{false};
        bool m_closed{false};
        bool m_terminated{false};
        bool m_allowAutoTermination{true};
    };
} // namespace pTK

#endif // PTK_APPLICATION_HPP
