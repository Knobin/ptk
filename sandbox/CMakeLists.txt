project(SandboxApp)

add_executable(${PROJECT_NAME} SandboxApp.cpp)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE ${PTK_DEPENDENCIES} skia ptk)