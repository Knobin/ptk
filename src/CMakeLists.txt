
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

if (PTK_BUILD_TYPE MATCHES Debug)
    set(LOG_FILES Log.cpp)
endif()

if (WIN32)
    set(PLATFORM_FILES platform/win/WinPlatform.cpp
            platform/win/WinRasterContext.cpp
            platform/win/WinBackend.cpp)
    if (HW_ACCELERATION)
        set(PLATFORM_FILES ${PLATFORM_FILES}
                platform/win/WinGLContext.cpp)
    endif (HW_ACCELERATION)
    message(STATUS "Using Windows backend")
else()
    set(PLATFORM_FILES platform/glfw/GLContext.cpp
            platform/glfw/GLFWBackend.cpp)
    message(STATUS "Using GLFW backend")
endif()

set(SOURCE_FILES Application.cpp Window.cpp
        ${CORE_FILES}
        ${PLATFORM_FILES}
        ${UTIL_FILES}
        ${WIDGET_FILES}
        ${LOG_FILES})
source_group(TREE "${CMAKE_SOURCE_DIR}/src" PREFIX "Source Files" FILES ${SOURCE_FILES})

# Define library
add_library(${PROJECT_NAME} STATIC ${HEADER_FILES} ${SOURCE_FILES})

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

# Skia
target_link_libraries(${PROJECT_NAME} ${SKIA_PLATFORM_DEPENDENCIES} skia)

# OpenGL
if (HW_ACCELERATION)
    target_include_directories(${PROJECT_NAME} PRIVATE ${OPENGL_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES})
endif (HW_ACCELERATION)

# GLFW
if (NOT WIN32)
    target_link_libraries(${PROJECT_NAME} glfw ${GLFW_LIBRARIES})
endif (NOT WIN32)

# spdlog
if (PTK_BUILD_TYPE MATCHES Debug)
    target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/spdlog/include)
endif()

# Threads
target_link_libraries(${PROJECT_NAME} Threads::Threads)
