set(TEST_NAME test_sofl_viewers)
add_executable(${TEST_NAME}
        ../TestSuiteMain.cpp
        TestSoFlPlaneViewer.cpp)
target_compile_definitions(${TEST_NAME} PRIVATE -DSOFL_INTERNAL=1)
target_link_libraries(${TEST_NAME}  PRIVATE SoFl)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
