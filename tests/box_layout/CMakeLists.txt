project(BoxLayoutTest)

add_executable(${PROJECT_NAME} BoxLayoutTest.cpp)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
)

target_compile_definitions(${PROJECT_NAME} PRIVATE ${PTK_DEFINITIONS})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} PRIVATE ${PTK_DEPENDENCIES} skia ptk)
