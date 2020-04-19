project(SandboxApp)

add_executable(SandboxApp SandboxApp.cpp)

# Set runtime library to MT or MTd (Release / Debug)
set_property(TARGET ${PROJECT_NAME} PROPERTY
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(SandboxApp ${SKIA_PLATFORM_DEPENDENCIES} skia ptk)
