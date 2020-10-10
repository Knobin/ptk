project(SandboxApp)

add_executable(${PROJECT_NAME} SandboxApp.cpp)

# Set runtime library to MT or MTd (Release / Debug)
set_property(TARGET ${PROJECT_NAME} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} ${PTK_DEPENDENCIES} skia ptk)
