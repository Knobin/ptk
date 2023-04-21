
set(PTK_INCLUDE "${CMAKE_SOURCE_DIR}/include")
set(PTK_SRC "${CMAKE_SOURCE_DIR}/src")

# PTK Header files
file(GLOB_RECURSE PTK_HEADER_FILES ${PTK_INCLUDE}/ptk/*.hpp)
source_group(TREE "${PTK_INCLUDE}" PREFIX "PTK Header Files" FILES ${PTK_HEADER_FILES})
include_directories(${PTK_INCLUDE})

# Function for define tests.
function(define_test)
    cmake_parse_arguments(
        TEST_PREFIX
        ""
        "NAME"
        "ARGS;FILES;LINKS;DEFINITIONS"
        ${ARGN}
    )

    if (TEST_PREFIX_NAME)
        if (TEST_PREFIX_FILES)
            add_executable(${TEST_PREFIX_NAME} ${TEST_PREFIX_FILES})
            if (TEST_PREFIX_DEFINITIONS)
              target_compile_definitions(${TEST_PREFIX_NAME} PRIVATE ${TEST_PREFIX_DEFINITIONS})
            endif (TEST_PREFIX_DEFINITIONS)
             set_target_properties(${TEST_PREFIX_NAME}
                PROPERTIES
                    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/lib
                    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/$<CONFIG>/bin
            )
            if (TEST_PREFIX_LINKS)
                target_link_libraries(${TEST_PREFIX_NAME} PRIVATE ${TEST_PREFIX_LINKS} Catch2::Catch2WithMain)
            else (TEST_PREFIX_LINKS)
                target_link_libraries(${TEST_PREFIX_NAME} PRIVATE Catch2::Catch2WithMain)
            endif (TEST_PREFIX_LINKS)
            add_test(NAME ${TEST_PREFIX_NAME} COMMAND ${TEST_PREFIX_NAME} ${TEST_PREFIX_ARGS})
        else (TEST_PREFIX_FILES)
            message(SEND_ERROR "No files specified for ${TEST_PREFIX_NAME}")
        endif (TEST_PREFIX_FILES)
    else (TEST_PREFIX_NAME)
        message(SEND_ERROR "No name specified for test!")
    endif (TEST_PREFIX_NAME)
endfunction()

# Add tests here!
define_test(NAME AlignmentTest FILES ${PTK_HEADER_FILES} AlignmentTest.cpp LINKS ptk DEFINITIONS ${PTK_DEFINITIONS})
define_test(NAME CallbackStorageTest FILES ${PTK_HEADER_FILES} CallbackStorageTest.cpp LINKS ptk DEFINITIONS ${PTK_DEFINITIONS})
define_test(NAME ColorTest FILES ${PTK_INCLUDE}/ptk/util/Color.hpp ${PTK_SRC}/util/Color.cpp ColorTest.cpp)
define_test(NAME PointTest FILES ${PTK_INCLUDE}/ptk/util/Point.hpp ${PTK_SRC}/util/Point.cpp PointTest.cpp)
define_test(NAME SafeQueueTest FILES ${PTK_INCLUDE}/ptk/util/SafeQueue.hpp SafeQueueTest.cpp)
define_test(NAME SemaphoreTest FILES ${PTK_INCLUDE}/ptk/util/Semaphore.hpp ${PTK_SRC}/util/Semaphore.cpp SemaphoreTest.cpp LINKS Threads::Threads)
define_test(NAME SizableTest FILES ${PTK_HEADER_FILES} SizableTest.cpp LINKS ptk DEFINITIONS ${PTK_DEFINITIONS})
define_test(NAME SizeTest FILES ${PTK_INCLUDE}/ptk/util/Size.hpp ${PTK_SRC}/util/Size.cpp SizeTest.cpp)
define_test(NAME SizePolicyTest FILES ${PTK_INCLUDE}/ptk/util/SizePolicy.hpp SizePolicyTest.cpp)
define_test(NAME Vec2Test FILES ${PTK_INCLUDE}/ptk/util/Vec2.hpp Vec2Test.cpp)
define_test(NAME WidgetTest FILES ${PTK_HEADER_FILES} WidgetTest.cpp LINKS ptk DEFINITIONS ${PTK_DEFINITIONS})
