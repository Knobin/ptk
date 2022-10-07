#
# Source files
#

set(PTK_CORE_FILES core/Alignment.cpp
        core/CallbackStorage.cpp
        core/EventCallbacks.cpp
        core/Shape.cpp
        core/Sizable.cpp
        core/Text.cpp
        core/Widget.cpp
        core/WidgetContainer.cpp)

set(PTK_EVENT_FILES events/KeyMap.cpp)

set(PTK_MENU_FILES menu/CheckboxMenuItem.cpp
        menu/Menu.cpp
        menu/MenuBar.cpp
        menu/MenuItem.cpp
        menu/NamedMenuItem.cpp)

set(PTK_UTIL_FILES util/Color.cpp
        util/Point.cpp
        util/Semaphore.cpp
        util/Size.cpp)

set(PTK_WIDGET_FILES widgets/Button.cpp
        widgets/Checkbox.cpp
        widgets/HBox.cpp
        widgets/Image.cpp
        widgets/Label.cpp
        widgets/Rectangle.cpp
        widgets/TextField.cpp
        widgets/VBox.cpp)

if (PTK_BUILD_TYPE MATCHES Debug)
    set(PTK_LOG_FILES Log.cpp)
endif()

# Get platform files
add_subdirectory(${CMAKE_SOURCE_DIR}/src/platform)

# Source files
set(PTK_SOURCE_FILES Application.cpp Window.cpp
        ${PTK_CORE_FILES}
        ${PTK_EVENT_FILES}
        ${PTK_MENU_FILES}
        ${PTK_UTIL_FILES}
        ${PTK_WIDGET_FILES}
        ${PTK_LOG_FILES})
list(TRANSFORM PTK_SOURCE_FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")
set(PTK_SOURCE_FILES ${PTK_SOURCE_FILES} ${PTK_PLATFORM_FILES} PARENT_SCOPE)
        
# All source files (including all platform files)
set(PTK_SOURCE_FILES_ALL ${PTK_SOURCE_FILES} ${PTK_PLATFORM_FILES_ALL} PARENT_SCOPE)
