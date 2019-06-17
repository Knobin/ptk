cmake_minimum_required(VERSION 3.10)
project(ptk_example)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic")

add_executable(${PROJECT_NAME} example.cpp)
target_link_libraries(${PROJECT_NAME} ptk)
