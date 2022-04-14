#
# Source files
#

set(CORE_FILES core/CallbackStorage.cpp
        core/EventCallbacks.cpp
        core/EventManager.cpp
        core/Shape.cpp
        core/Sizable.cpp
        core/Text.cpp
        core/Widget.cpp
        core/WidgetContainer.cpp)

set(EVENT_FILES events/KeyMap.cpp)

set(MENU_FILES menu/CheckboxMenuItem.cpp
        menu/Menu.cpp
        menu/MenuBar.cpp
        menu/MenuItem.cpp
        menu/NamedMenuItem.cpp)

set(UTIL_FILES util/Alignment.cpp
        util/Color.cpp
        util/Point.cpp
        util/Semaphore.cpp
        util/Size.cpp)

set(WIDGET_FILES widgets/Button.cpp
        widgets/Checkbox.cpp
        widgets/HBox.cpp
        widgets/Image.cpp
        widgets/Label.cpp
        widgets/Rectangle.cpp
        widgets/TextField.cpp
        widgets/VBox.cpp)

if (PTK_BUILD_TYPE MATCHES Debug)
    set(LOG_FILES Log.cpp)
endif()

# Get platform files
add_subdirectory(${CMAKE_SOURCE_DIR}/src/platform)

# Header files
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/ptk/*.hpp)
source_group(TREE "${CMAKE_SOURCE_DIR}/include/ptk" PREFIX "Header Files" FILES ${HEADER_FILES})

# Source files
set(SOURCE_FILES Application.cpp Window.cpp
        ${CORE_FILES}
        ${PLATFORM_FILES}
        ${EVENT_FILES}
        ${MENU_FILES}
        ${UTIL_FILES}
        ${WIDGET_FILES}
        ${LOG_FILES})
set(SOURCE_FILES_ALL ${SOURCE_FILES}
    ${PLATFORM_FILES_ALL})
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCE_FILES_ALL})

# Define library
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")

# Set runtime library to MT or MTd (Release / Debug)
set_property(TARGET ${PROJECT_NAME} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Compiler flags for ptk
if (MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC /permissive- /W4 /w14640)
else()
    target_compile_options(${PROJECT_NAME} PUBLIC
            -Wall -Wextra -Wpedantic -fvisibility=hidden -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align
            -Wunused -Woverloaded-virtual -Wconversion -Wsign-conversion -Wdouble-promotion -Wformat=2)
endif ()

# includes and deps
target_include_directories(${PROJECT_NAME} PRIVATE ${PTK_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${PTK_DEPENDENCIES})

# Skia
target_link_libraries(${PROJECT_NAME} skia)

# OpenGL
if (ENABLE_OPENGL)
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif (ENABLE_OPENGL)

# spdlog
if (PTK_BUILD_TYPE MATCHES Debug)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/spdlog/include)
endif()

# Threads
target_link_libraries(${PROJECT_NAME} Threads::Threads)
