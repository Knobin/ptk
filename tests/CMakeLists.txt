cmake_minimum_required(VERSION 3.10)

set(PTK_INCLUDE "${CMAKE_SOURCE_DIR}/include/ptk")
set(PTK_SRC "${CMAKE_SOURCE_DIR}/src")

# PTK Header files
file(GLOB_RECURSE HEADER_FILES ${CMAKE_SOURCE_DIR}/include/*.hpp)
source_group(TREE "${PTK_INCLUDE}" PREFIX "PTK Header Files" FILES ${PTK_HEADER_FILES})

# Function for define tests.
function(define_test)
    cmake_parse_arguments(
        TEST_PREFIX
        ""
        "NAME"
        "ARGS;FILES;LINKS"
        ${ARGN}
    )

    if (TEST_PREFIX_NAME)
        if (TEST_PREFIX_FILES)
            add_executable(${TEST_PREFIX_NAME} ${TEST_PREFIX_FILES})
            if (TEST_PREFIX_LINKS)
                target_link_libraries(${TEST_PREFIX_NAME} ${TEST_PREFIX_LINKS})
            endif (TEST_PREFIX_LINKS)
            target_include_directories(${TEST_PREFIX_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/third_party/Catch2/single_include)
            add_test(NAME ${TEST_PREFIX_NAME} COMMAND ${TEST_PREFIX_NAME} ${TEST_PREFIX_ARGS})
        else (TEST_PREFIX_FILES)
            message(SEND_ERROR "No files specified for ${TEST_PREFIX_NAME}")
        endif (TEST_PREFIX_FILES)
    else (TEST_PREFIX_NAME)
        message(SEND_ERROR "No name specified for test!")
    endif (TEST_PREFIX_NAME)
endfunction()

# Add tests here!
define_test(NAME ClockTest FILES ${PTK_INCLUDE}/util/Clock.hpp ${PTK_SRC}/util/Clock.cpp ClockTest.cpp)
define_test(NAME ColorTest FILES ${PTK_INCLUDE}/util/Color.hpp ${PTK_SRC}/util/Color.cpp ColorTest.cpp)
define_test(NAME PointTest FILES ${PTK_INCLUDE}/util/Point.hpp ${PTK_SRC}/util/Point.cpp PointTest.cpp)
define_test(NAME RectangleTest FILES ${PTK_HEADER_FILES} RectangleTest.cpp LINKS ptk)
define_test(NAME SafeQueueTest FILES ${PTK_INCLUDE}/util/SafeQueue.hpp SafeQueueTest.cpp)
define_test(NAME SemaphoreTest FILES ${PTK_INCLUDE}/util/Semaphore.hpp ${PTK_SRC}/util/Semaphore.cpp SemaphoreTest.cpp LINKS Threads::Threads)
define_test(NAME ShapeTest FILES ${PTK_HEADER_FILES} ShapeTest.cpp LINKS ptk)
define_test(NAME SizableTest FILES ${PTK_HEADER_FILES} SizableTest.cpp LINKS ptk)
define_test(NAME SizeTest FILES ${PTK_INCLUDE}/util/Size.hpp ${PTK_SRC}/util/Size.cpp SizeTest.cpp)
define_test(NAME Vec2Test FILES ${PTK_INCLUDE}/util/Vec2.hpp Vec2Test.cpp)
define_test(NAME WidgetTest FILES ${PTK_HEADER_FILES} WidgetTest.cpp LINKS ptk)
