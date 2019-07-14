cmake_minimum_required(VERSION 3.10)

# Source files
file(GLOB TEST_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

foreach(FILE ${TEST_SOURCE_FILES})
    get_filename_component(WITHOUT_PATH ${FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" EXECUTABLE ${WITHOUT_PATH})
    add_executable(${EXECUTABLE} ${FILE})
    target_link_libraries(${EXECUTABLE} ${SKIA_PLATFORM_DEPENDENCIES} skia ptk)
    target_include_directories(${EXECUTABLE} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/Catch2/single_include)
    add_test(NAME ${EXECUTABLE} COMMAND ${EXECUTABLE})
    set(TEST_LIST "${TEST_LIST}${EXECUTABLE};")
endforeach()

message(STATUS "Tests: ${TEST_LIST}")
