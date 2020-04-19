
# Source files
set(CORE_FILES core/Box.cpp
        core/ContextBase.cpp
        core/EventCallbacks.cpp
        core/EventHandling.cpp
        core/Font.cpp
        core/Shape.cpp
        core/Sizable.cpp
        core/Text.cpp
        core/Widget.cpp
        )

set(UTIL_FILES util/Alignment.cpp
        util/Clock.cpp
        util/Color.cpp
        util/Point.cpp
        util/Semaphore.cpp
        util/Size.cpp)

set(WIDGET_FILES widgets/Button.cpp
        widgets/Cell.cpp
        widgets/Checkbox.cpp
        widgets/HBox.cpp
        widgets/Image.cpp
        widgets/Label.cpp
        widgets/Rectangle.cpp
        widgets/VBox.cpp)

if (WIN32)
    set(PLATFORM_FILES platform/win32/Win32RasterContext.cpp
            platform/win32/Win32Backend.cpp)
    message(STATUS "Using Win32")
else()
    set(PLATFORM_FILES platform/glfw/GLContext.cpp
            platform/glfw/GLFWBackend.cpp)
    message(STATUS "Using GLFW")
endif()

set(SOURCE_FILES Application.cpp Log.cpp Window.cpp
        ${CORE_FILES}
        ${PLATFORM_FILES}
        ${UTIL_FILES}
        ${WIDGET_FILES})
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCE_FILES})

# Define library
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

# Set runtime library to MT or MTd (Release / Debug)
set_property(TARGET ${PROJECT_NAME} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Compiler flags for ptk
if (NOT MSVC)
    target_compile_options(${PROJECT_NAME} PUBLIC
            -Wall -Wextra -Wpedantic -fvisibility=hidden)
endif (NOT MSVC)

# Skia
target_link_libraries(${PROJECT_NAME} ${SKIA_PLATFORM_DEPENDENCIES} skia)

# OpenGL
target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})

# GLFW
if (NOT WIN32)
    target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
endif (NOT WIN32)

# spdlog
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/spdlog/include)

# Threads
target_link_libraries(${PROJECT_NAME} Threads::Threads)
