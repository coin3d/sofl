# sigsev on exit to be investigated
if(SOFL_ALL_TESTS)
    set(TEST_NAME test_sofl_widgets)
    add_executable(${TEST_NAME} ../TestSuiteMain.cpp TestSoFlGLWidgetP.cpp ) #TestSoFlGLArea.cpp)
    target_compile_definitions(${TEST_NAME} PRIVATE -DSOFL_INTERNAL=1)
    target_link_libraries(${TEST_NAME}  PRIVATE SoFl
            ${FLTK_LIBRARIES}
            ${OPENGL_LIBRARIES})

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endif ()