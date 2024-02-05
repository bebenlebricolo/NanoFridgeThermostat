set(TEST_NAME bridge_test)

add_executable(${TEST_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge_tests.cpp
)

gtest_discover_tests(${TEST_NAME})

target_include_directories(${TEST_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(${TEST_NAME}
    bridge
    GTest::gtest
)