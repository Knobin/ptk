
# Source files
file(GLOB TEST_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(FILE ${TEST_SOURCE_FILES})
    get_filename_component(WITHOUT_PATH ${FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" EXECUTABLE ${WITHOUT_PATH})
    add_executable(${EXECUTABLE} ${FILE})
    set_target_properties(${EXECUTABLE}
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
    )
    target_compile_definitions(${EXECUTABLE} PRIVATE ${PTK_DEFINITIONS})
    target_include_directories(${EXECUTABLE} PRIVATE "${CMAKE_SOURCE_DIR}/include")
    target_link_libraries(${EXECUTABLE} ${PTK_DEPENDENCIES} skia ptk)
    set(EXAMPLE_LIST "${EXAMPLE_LIST}${EXECUTABLE};")
endforeach()
