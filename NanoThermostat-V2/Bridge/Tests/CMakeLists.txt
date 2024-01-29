add_executable(bridge_test
    ${CMAKE_CURRENT_SOURCE_DIR}/bridge_tests.cpp
)

gtest_discover_tests(bridge_test)

target_include_directories(bridge_test
    SYSTEM PUBLIC
        ${GTEST_INCLUDE_DIRS}
    PRIVATE
        ${CMAKE_SOURCE_DIR}
)

target_link_libraries(bridge_test
    bridge
    ${GTEST_LIBRARIES}
)
