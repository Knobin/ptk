cmake_minimum_required(VERSION 3.10)
project(ptk_example)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

# Source files
 file(GLOB TEST_SOURCE_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)
    
 foreach(FILE ${TEST_SOURCE_FILES})
    get_filename_component(WITHOUT_PATH ${FILE} NAME)
    string(REGEX REPLACE "\\.[^.]*$" "" EXECUTABLE ${WITHOUT_PATH})
    add_executable(${EXECUTABLE} ${FILE})
    target_link_libraries(${EXECUTABLE} ptk)
 endforeach()
