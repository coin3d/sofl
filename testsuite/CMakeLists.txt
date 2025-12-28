RETURN()

enable_testing()

include_directories(
        $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${fltk_INCLUDE_DIRS}
)

add_subdirectory(devices)
add_subdirectory(widgets)

set(TEST_NAME test_sofl)
add_executable(${TEST_NAME} TestSuiteMain.cpp TestSoFl.cpp)
target_link_libraries(${TEST_NAME}  SoFl )

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})