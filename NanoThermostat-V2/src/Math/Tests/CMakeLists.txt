set(TEST_NAME interpolation_test)

add_executable(${TEST_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/interpolation_test.cpp
)

gtest_discover_tests(${TEST_NAME})

target_include_directories(${TEST_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${TEST_NAME}
    mathlib
    GTest::gtest
)