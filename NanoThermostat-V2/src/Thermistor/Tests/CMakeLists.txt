set(TEST_NAME thermistor_tests)

add_executable(${TEST_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/thermistor_test.cpp
)

gtest_discover_tests(${TEST_NAME})

target_include_directories(${TEST_NAME} SYSTEM PUBLIC
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${TEST_NAME}
    thermistor
    mathlib
    GTest::gtest
)