cmake_minimum_required(VERSION 3.10)

add_executable(SandboxApp SandboxApp.cpp)
target_link_libraries(SandboxApp ${SKIA_PLATFORM_DEPENDENCIES} skia ptk)

