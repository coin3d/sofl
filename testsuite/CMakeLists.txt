# Set CMP0167 to NEW to use Boost Config Mode and suppress deprecation warnings
if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

# Find Boost using Config Mode
find_package(Boost COMPONENTS unit_test_framework CONFIG)

# Fallback to module mode if Config Mode fails
if(NOT Boost_FOUND)
    find_package(Boost COMPONENTS unit_test_framework)
endif()


if( Boost_UNIT_TEST_FRAMEWORK_FOUND OR Boost_FOUND)

    include_directories(
            $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/src>
            $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${fltk_INCLUDE_DIRS}
    )

    add_subdirectory(devices)
    add_subdirectory(widgets)
    add_subdirectory(viewers)

    set(TEST_NAME test_sofl)
    add_executable(${TEST_NAME} TestSuiteMain.cpp TestSoFl.cpp)
    target_link_libraries(${TEST_NAME}  SoFl )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
else()
    message(WARNING "Boost Unit Test Framework not found. Skipping TestSoFlGLWidgetP.")
endif()
